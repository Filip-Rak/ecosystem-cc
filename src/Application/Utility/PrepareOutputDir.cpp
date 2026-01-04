#include "Application/Utility/PrepareOutputDir.hpp"

#include <filesystem>
#include <fstream>
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

auto handleDirectory( const filesystem::path& outputPath, const bool overwrite ) -> std::optional< InitError >
{
	if ( filesystem::exists( outputPath ) )
	{
		if ( !filesystem::is_directory( outputPath ) )
			return "-> " + outputPath.relative_path().string() + " is not a directory";

		if ( overwrite )
		{
			for ( const auto& entry : filesystem::directory_iterator( outputPath ) )
			{
				filesystem::remove_all( entry.path() );
			}
		}
		else if ( !filesystem::is_empty( outputPath ) )
		{
			return "-> " + outputPath.relative_path().string() +
			       " is not empty. Directory may contain output files from previous runs. Use --overwrite to "
			       "ignore ";
		}
	}

	return std::nullopt;
}
}  // namespace

auto prepareOutputDir( const Preset& preset, const bool overwrite ) -> std::optional< InitError >
{
	const auto& outputPath = preset.outputDirectoryPath;

	try
	{
		if ( const auto error = handleDirectory( outputPath, overwrite ); error )
		{
			return *error;
		}

		const auto marker = outputPath / "incomplete-results.warning";
		filesystem::create_directories( outputPath );

		std::ofstream file( marker );
		if ( !file ) return "-> Failed to create marker file in output directory";

		copyResourceFile( preset.presetPath, preset.outputDirectoryPath );

		const auto& constants = constant::filePaths;
		const auto& grid      = preset.gridDirectoryPath;
		copyResourceFile( grid / constants.temperaturePath, preset.outputDirectoryPath );
		copyResourceFile( grid / constants.elevationPath, preset.outputDirectoryPath );
		copyResourceFile( grid / constants.humidityPath, preset.outputDirectoryPath );
	}
	catch ( const filesystem::filesystem_error& error )
	{
		return "-> Failed to prepare the output directory: " + std::string( error.what() );
	}

	return std::nullopt;
}
}  // namespace cc::app