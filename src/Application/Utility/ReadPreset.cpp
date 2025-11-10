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
		json = Njson::parse( file, nullptr, true, true );
	}
	catch ( const Njson::parse_error& exception )
	{
		return std::unexpected( "-> " + std::string( exception.what() ) );
	}

	constexpr const auto& Keys = constant::JsonKeys;
	return Preset{ .gridDirectoryPath = json[ Keys.GridDirectoryPath ] };
}
}  // namespace cc::app