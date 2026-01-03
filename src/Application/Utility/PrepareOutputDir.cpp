#include "Application/Utility/PrepareOutputDir.hpp"

#include <fstream>

namespace cc::app
{
namespace filesystem = std::filesystem;
auto prepareOutputDir( const filesystem::path& path, const bool overwrite ) -> std::optional< InitError >
{
	try
	{
		if ( filesystem::exists( path ) )
		{
			if ( !filesystem::is_directory( path ) )
				return "-> " + path.relative_path().string() + " is not a directory";
			if ( !overwrite && !filesystem::is_empty( path ) )
				return "-> " + path.relative_path().string() +
				       " is not empty. Directory may contain output files from the previous run. Use --overwrite to "
				       "ignore ";
		}

		const auto marker = path / "invalid-results.warning";
		filesystem::create_directories( path );

		std::ofstream file( marker );
		if ( !file ) return "-> Failed to create marker file in output directory";
	}
	catch ( const filesystem::filesystem_error& error )
	{
		return "-> Failed to create the output directory: " + std::string( error.what() );
	}

	return std::nullopt;
}
}  // namespace cc::app