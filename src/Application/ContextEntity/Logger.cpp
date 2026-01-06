#include "Application/ContextEntity/Logger.hpp"

#include <filesystem>
#include <fstream>

#include <memory>
#include <optional>
#include <print>
#include <string>
#include <vector>

#include <entt/entt.hpp>

#include "Application/Constants/FilePathConstants.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/TickLog.hpp"
#include "Application/Events/SimRunnerEvents.hpp"

namespace cc::app
{
namespace
{
namespace filesystem = std::filesystem;

const filesystem::path tickDataFile = "tickData.csv";
const filesystem::path markerFile   = "incomplete-results.warning";

auto copyResourceFile( const filesystem::path& resource, const filesystem::path& outputPath ) -> void
{
	filesystem::copy_file( resource, outputPath / resource.filename(), filesystem::copy_options::overwrite_existing );
}

[[nodiscard]] auto handleDirectory( const filesystem::path& outputPath, const bool clean ) -> std::optional< Error >
{
	if ( filesystem::exists( outputPath ) )
	{
		if ( !filesystem::is_directory( outputPath ) )
		{
			return "-> " + outputPath.relative_path().string() + " is not a directory";
		}

		if ( clean )
		{
			for ( const auto& entry : filesystem::directory_iterator( outputPath ) )
				filesystem::remove_all( entry.path() );
		}
		else if ( !filesystem::is_empty( outputPath ) )
		{
			return "-> " + outputPath.relative_path().string() +
			       " is not empty. Directory may contain output files from previous runs. Use --clean to "
			       "clean the directory first instead ";
		}
	}

	return std::nullopt;
}

[[nodiscard]] auto prepareOutputDir( const Preset& preset, const bool clean ) -> std::optional< Error >
{
	const auto& log        = preset.logging;
	const auto& outputPath = log.outputDirectoryPath;

	try
	{
		if ( const auto error = handleDirectory( outputPath, clean ); error )
		{
			return *error;
		}

		const auto marker = outputPath / markerFile;
		filesystem::create_directories( outputPath );

		std::ofstream file( marker );
		if ( !file ) return "-> Failed to create marker file in output directory";

		copyResourceFile( preset.presetPath, log.outputDirectoryPath );

		const auto& constants = constant::filePaths;
		const auto& grid      = preset.gridDirectoryPath;
		copyResourceFile( grid / constants.temperaturePath, log.outputDirectoryPath );
		copyResourceFile( grid / constants.elevationPath, log.outputDirectoryPath );
		copyResourceFile( grid / constants.humidityPath, log.outputDirectoryPath );
	}
	catch ( const filesystem::filesystem_error& error )
	{
		return "-> Failed to prepare the output directory: " + std::string( error.what() );
	}

	return std::nullopt;
}

auto removeMarker( const filesystem::path& outputPath ) -> void
{
	try
	{
		filesystem::remove( outputPath / markerFile );
	}
	catch ( const filesystem::filesystem_error& error )
	{
		std::print( "Failed to remove the marker file - output is valid. Reason: {}", error.what() );
	}
}

auto dumpData( std::ofstream& file, std::string& data )
{
	file << data;
	data.clear();
}

}  // namespace

Logger::Logger( entt::registry& registry ) : m_registry( registry )
{
	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::ResetSim >().connect< &Logger::onResetSim >( *this );
	dispatcher.sink< event::Extinction >().connect< &Logger::onExtinction >( *this );
	dispatcher.sink< event::ReachedTargetIteration >().connect< &Logger::onReachedTargetIteration >( *this );
}

Logger::~Logger()
{
	for ( auto& out : m_outputData )
	{
		dumpData( out->file, out->pendingData );
		out->file.flush();
		out->file.close();
	}
}

auto Logger::init( const bool clean ) -> std::optional< Error >
{
	const auto& preset = m_registry.ctx().get< Preset >();
	if ( const auto error = prepareOutputDir( preset, clean ); error )
	{
		return "-> Unable to save output data\n" + *error;
	}

	const auto& outputPath = preset.logging.outputDirectoryPath;
	if ( preset.logging.logPerTickState )
	{
		m_outputData.emplace_back( std::make_unique< OutputData >() );
		m_tickData = m_outputData.back().get();

		auto& tickData     = m_outputData.back();
		tickData->filename = tickDataFile;

		tickData->file.open( outputPath / tickDataFile );
		if ( !tickData->file.good() )
		{
			return "-> Couldn't create output file\n";
		}
		constexpr auto megabyte = 1048576uz;
		tickData->pendingData.reserve( megabyte );

		std::format_to( std::back_inserter( tickData->pendingData ),
		                "iteration,liveAgents,births,starvations,ageDeaths,moves,occupiedCells,"
		                "vegetationEaten,vegetationTotal,meanVegetation,meanCrowd,"
		                "meanEnergy,energyVar,energyP50,energyP90,"
		                "meanEnergyGene,energyGeneVar,energyGeneP50,energyGeneP90,"
		                "meanTempAdaptation,meanHumAdaptation,meanElevAdaptation" );
	}

	return std::nullopt;
}

auto Logger::writeTickData( const TickLog& tick ) -> void
{
	if ( m_collectingDone || m_tickData == nullptr ) return;

	auto& buffer = m_tickData->pendingData;
	std::format_to( std::back_inserter( buffer ),
	                "\n{},{},{},{},{},{},{},"
	                "{:.3f},{:.3f},{:.3f},{:.3f},"
	                "{:.3f},{:.3f},{:.3f},{:.3f},"
	                "{:.3f},{:.3f},{:.3f},{:.3f},"
	                "{:.3f},{:.3f},{:.3f}",
	                tick.iteration, tick.liveAgents, tick.births, tick.starvations, tick.ageDeaths, tick.moves,
	                tick.occupiedCells, tick.vegetationEaten, tick.vegetationTotal, tick.meanVegetation, tick.meanCrowd,
	                tick.meanEnergy, tick.energyVar, tick.energyP50, tick.energyP90, tick.meanEnergyGene,
	                tick.energyGeneVar, tick.energyGeneP50, tick.energyGeneP90, tick.meanTempAdaptation,
	                tick.meanHumAdaptation, tick.meanElevAdaptation );

	m_tickData->file.flush();
	constexpr auto flushRate = 0.9f;
	if ( static_cast< float >( buffer.size() ) >= static_cast< float >( buffer.capacity() ) * flushRate )
	{
		dumpData( m_tickData->file, m_tickData->pendingData );
	}
}

auto Logger::dumpDataAndClose() -> void
{
	m_collectingDone = true;

	for ( auto& out : m_outputData )
	{
		dumpData( out->file, out->pendingData );
		out->file.flush();
		out->file.close();
	}
}

auto Logger::onResetSim( const event::ResetSim& /*event*/ ) -> void
{
	if ( m_collectingDone ) return;

	const auto& outputPath = m_registry.ctx().get< Preset >().logging.outputDirectoryPath;
	for ( auto& out : m_outputData )
	{
		out->file.open( outputPath / out->filename );
		out->pendingData.clear();
	}
}

auto Logger::onReachedTargetIteration( const event::ReachedTargetIteration& /*event*/ ) -> void
{
	dumpDataAndClose();

	const auto& outputPath = m_registry.ctx().get< Preset >().logging.outputDirectoryPath;
	removeMarker( outputPath );
}

auto Logger::onExtinction( const event::Extinction& /*event*/ ) -> void
{
	dumpDataAndClose();
}
}  // namespace cc::app