#include "Application/Utility/ReadPreset.hpp"

#include <cstddef>
#include <fstream>

#include <nlohmann/json.hpp>

#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
auto readPreset( const std::filesystem::path& path ) -> std::expected< Preset, ParsingError >
{
	using njson = nlohmann::json;

	std::ifstream file( path );
	if ( !file.is_open() )
	{
		return std::unexpected( "-> Couldn't open file: " + path.string() );
	}

	njson json;
	try
	{
		constexpr bool allowExceptions    = true;
		constexpr bool allowComments      = true;
		constexpr std::nullptr_t callback = nullptr;

		json = njson::parse( file, callback, allowExceptions, allowComments );
	}
	catch ( const njson::parse_error& exception )
	{
		return std::unexpected( "-> " + std::string( exception.what() ) );
	}

	std::string lastTriedKey;
	try
	{
		lastTriedKey        = "gridDirectoryPath";
		const auto gridPath = json[ lastTriedKey ];

		lastTriedKey               = "iterationTarget";
		const auto iterationTarget = json[ lastTriedKey ];

		lastTriedKey       = "rngSeed";
		const auto rngSeed = json[ lastTriedKey ];

		return Preset{ .presetName        = path.filename().string(),
		               .gridDirectoryPath = gridPath,
		               .iterationTarget   = iterationTarget,
		               .rngSeed           = rngSeed };
	}
	catch ( const njson::exception& exception )
	{
		return std::unexpected( "-> Couldn't parse preset\n-> Affected key: " + lastTriedKey +
		                        "\n-> Issue: " + std::string( exception.what() ) );
	}
}
}  // namespace cc::app