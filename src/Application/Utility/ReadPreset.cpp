#include "Application/Utility/ReadPreset.hpp"

#include <cstddef>
#include <exception>
#include <fstream>

#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <string_view>

#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
namespace
{

using njson = nlohmann::json;

auto getErrorMessage( const std::string_view& path, const std::string& key, const std::exception& exception ) -> std::runtime_error
{
	return std::runtime_error( "\n-> Path: " + std::string( path ) + "\n-> Key: " + key +
	                           "\n-> Issue: " + std::string( exception.what() ) );
}

template < class T >
auto get( const njson& json, const std::string_view& path ) -> T
{
	const njson* current = &json;

	std::size_t index = 0;
	std::string key;

	while ( index < path.size() )
	{
		const auto slash   = path.find( '/', index );
		const auto keyView = path.substr( index, ( slash == std::string_view::npos ) ? ( path.size() - index ) : ( slash - index ) );

		key = std::string( keyView );

		try
		{
			current = &current->at( key.c_str() );
		}
		catch ( const std::exception& exception )
		{
			throw getErrorMessage( path, key, exception );
		}

		if ( slash == std::string_view::npos ) break;
		index = slash + 1;
	}

	try
	{
		return current->get< T >();
	}
	catch ( const std::exception& exception )
	{
		throw getErrorMessage( path, key, exception );
	}
}
}  // namespace

auto readPreset( const std::filesystem::path& path ) -> std::expected< Preset, ParsingError >
{
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

	try
	{
		// Misc
		const auto gridPath        = get< std::string >( json, "gridDirectoryPath" );
		const auto iterationTarget = get< std::size_t >( json, "iterationTarget" );
		const auto rngSeed         = get< std::uint16_t >( json, "rngSeed" );

		// Vegetation - speed
		Preset::Vegetation::Speed speed{
		    .speedTempOptimal = get< float >( json, "vegetation/speed/tempOptimal" ),
		    .speedTempWidth   = get< float >( json, "vegetation/speed/tempWidth" ),
		    .speedHumOptimal  = get< float >( json, "vegetation/speed/humOptimal" ),
		    .speedHumWidth    = get< float >( json, "vegetation/speed/humWidth" ),
		};

		// Vegetation - limit
		Preset::Vegetation::Limit limit{
		    .limitTempOptimal   = get< float >( json, "vegetation/limit/tempOptimal" ),
		    .limitTempWidth     = get< float >( json, "vegetation/limit/tempWidth" ),
		    .limitHumOptimal    = get< float >( json, "vegetation/limit/humOptimal" ),
		    .limitHumWidth      = get< float >( json, "vegetation/limit/humWidth" ),
		    .limitElevHalf      = get< float >( json, "vegetation/limit/elevHalf" ),
		    .limitElevSteepness = get< float >( json, "vegetation/limit/elevSteepness" ),
		};

		return Preset{
		    .vegetation{ .speed = speed, .limit = limit },
		    .presetName        = path.filename().string(),
		    .gridDirectoryPath = gridPath,
		    .iterationTarget   = iterationTarget,
		    .rngSeed           = rngSeed,
		};
	}
	catch ( const std::exception& exception )
	{
		return std::unexpected( "-> Parsing error" + std::string( exception.what() ) );
	}
}
}  // namespace cc::app