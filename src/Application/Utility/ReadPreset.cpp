#include "Application/Utility/ReadPreset.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

namespace cc::app
{
using Njson = nlohmann::json;

namespace constant
{
struct JsonKeys_t
{
	const std::string_view GridDirectoryPath = "gridDirectoryPath";
} inline constexpr JsonKeys;
}  // namespace constant

auto readPreset( const std::filesystem::path& path ) -> std::expected< Preset, ParsingError >
{
	std::ifstream file( path );
	if ( !file.is_open() )
	{
		return std::unexpected( "-> Couldn't open file: " + path.string() );
	}

	Njson json;
	try
	{
		constexpr bool AllowExceptions = true;
		constexpr bool AllowComments = true;
		json = Njson::parse( file, nullptr, AllowExceptions, AllowComments );
	}
	catch ( const Njson::parse_error& exception )
	{
		return std::unexpected( "-> " + std::string( exception.what() ) );
	}

	// TODO: When adding more options run this try catch for each property via function for good log accuracy
	try
	{
		constexpr const auto& Keys = constant::JsonKeys;
		return Preset{ .gridDirectoryPath = json[ Keys.GridDirectoryPath ] };
	}
	catch ( const nlohmann::json::exception& exception )
	{
		return std::unexpected( "-> " + std::string( exception.what() ) );
	}
}
}  // namespace cc::app