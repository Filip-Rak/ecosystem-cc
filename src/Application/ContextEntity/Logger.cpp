#include "Application/ContextEntity/Logger.hpp"

#include <filesystem>
#include <fstream>

#include <entt/entt.hpp>
#include <optional>
#include <print>
#include <string>

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
			return "-> " + outputPath.relative_path().string() + " is not a directory";

		if ( clean )
		{
			for ( const auto& entry : filesystem::directory_iterator( outputPath ) )
			{
				filesystem::remove_all( entry.path() );
			}
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
	file.flush();
}

}  // namespace

Logger::Logger( entt::registry& registry ) : m_registry( registry )
{
	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::ResetSim >().connect< &Logger::onResetSim >( *this );
	dispatcher.sink< event::ReachedTargetIteration >().connect< &Logger::onReachedTargetIteration >( *this );
}

Logger::~Logger()
{
	dumpData( m_tickData.file, m_tickData.pendingData );
	m_tickData.file.close();
}

auto Logger::init( const bool clean ) -> std::optional< Error >
{
	const auto& preset = m_registry.ctx().get< Preset >();
	if ( const auto error = prepareOutputDir( preset, clean ); error )
	{
		return "-> Unable to save output data\n" + *error;
	}

	const auto& outputPath = preset.logging.outputDirectoryPath;

	m_tickData.file.open( outputPath / tickDataFile );
	if ( !m_tickData.file.good() )
	{
		return "-> Couldn't create output file\n";
	}
	constexpr auto megabyte = 1048576uz;
	m_tickData.pendingData.reserve( megabyte );

	std::format_to( std::back_inserter( m_tickData.pendingData ),
	                "iteration,liveAgents,births,starvations,ageDeaths,meanEnergy,meanTempAdaptation,meanHumAdaptation,"
	                "meanElevAdaptation\n" );

	return std::nullopt;
}

auto Logger::logTickData( const TickLog& t ) -> void
{
	if ( m_targetReached ) return;

	auto& buffer = m_tickData.pendingData;
	std::format_to( std::back_inserter( buffer ), "{},{},{},{},{},{:.3f},{:.3f},{:.3f},{:.3f}\n", t.iteration,
	                t.liveAgents, t.births, t.starvations, t.ageDeaths, t.meanEnergy, t.meanTempAdaptation,
	                t.meanHumAdaptation, t.meanElevAdaptation );
	m_tickData.file.flush();
	constexpr auto flushRate = 0.9f;
	if ( static_cast< float >( buffer.size() ) >= static_cast< float >( buffer.capacity() ) * flushRate )
	{
		dumpData( m_tickData.file, m_tickData.pendingData );
	}
}

auto Logger::onResetSim( const event::ResetSim& /*event*/ ) -> void
{
	if ( m_targetReached ) return;

	const auto& outputPath = m_registry.ctx().get< Preset >().logging.outputDirectoryPath;
	m_tickData.file.open( outputPath / tickDataFile );
	m_tickData.pendingData.clear();
}

auto Logger::onReachedTargetIteration( const event::ReachedTargetIteration& /*event*/ ) -> void
{
	m_targetReached = true;
	dumpData( m_tickData.file, m_tickData.pendingData );

	const auto& outputPath = m_registry.ctx().get< Preset >().logging.outputDirectoryPath;
	removeMarker( outputPath );
}
}  // namespace cc::app