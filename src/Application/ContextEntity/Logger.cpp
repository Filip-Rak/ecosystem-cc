#include "Application/ContextEntity/Logger.hpp"

#include <filesystem>
#include <fstream>

#include <entt/entt.hpp>
#include <optional>

#include "Application/Constants/FilePathConstants.hpp"
#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
namespace
{
namespace filesystem = std::filesystem;
auto copyResourceFile( const filesystem::path& resource, const filesystem::path& outputPath ) -> void
{
	filesystem::copy_file( resource, outputPath / resource.filename(), filesystem::copy_options::overwrite_existing );
}

auto handleDirectory( const filesystem::path& outputPath, const bool clean ) -> std::optional< Error >
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
auto prepareOutputDir( const Preset& preset, const bool clean ) -> std::optional< Error >
{
	const auto& log        = preset.logging;
	const auto& outputPath = log.outputDirectoryPath;

	try
	{
		if ( const auto error = handleDirectory( outputPath, clean ); error )
		{
			return *error;
		}

		const auto marker = outputPath / "incomplete-results.warning";
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
}  // namespace

Logger::Logger( entt::registry& registry ) : m_registry( registry ) {}

auto Logger::init( const bool clean ) -> std::optional< Error >
{
	const auto& preset = m_registry.ctx().get< Preset >();
	if ( const auto error = prepareOutputDir( preset, clean ); error )
	{
		return "-> Unable to save output data\n" + *error;
	}

	return std::nullopt;
}
}  // namespace cc::app