#include "Application/Utility/ReadPreset.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
using Njson = nlohmann::json;

namespace constant
{
struct JsonKeys_t
{
	const std::string_view GridDirectoryPath = "gridDirectoryPath";
	const std::string_view IterationTarget = "iterationTarget";
	const std::string_view RngSeed = "rngSeed";
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

	constexpr const auto& Keys = constant::JsonKeys;
	std::string lastKey = Keys.GridDirectoryPath.data();
	try
	{
		lastKey = Keys.GridDirectoryPath.data();
		const auto gridPath = json[ lastKey ];

		lastKey = Keys.IterationTarget.data();
		const auto iterationTarget = json[ lastKey ];

		lastKey = Keys.RngSeed.data();
		const auto RngSeed = json[ lastKey ];

		return Preset{ .presetName = path.filename().string(),
		               .gridDirectoryPath = gridPath,
		               .iterationTarget = iterationTarget,
		               .rngSeed = RngSeed };
	}
	catch ( const nlohmann::json::exception& exception )
	{
		return std::unexpected( "-> Couldn't parse preset\n-> Affected key: " + lastKey +
		                        "\n-> Issue: " + std::string( exception.what() ) );
	}
}
}  // namespace cc::app