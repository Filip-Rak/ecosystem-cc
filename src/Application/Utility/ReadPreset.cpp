#include "Application/Utility/ReadPreset.hpp"

#include <cstddef>
#include <exception>
#include <fstream>

#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
namespace
{

using njson = nlohmann::json;

template < class F >
[[nodiscard]] auto withPath( std::string_view path, F&& function ) -> decltype( auto )
{
	try
	{
		return std::forward< F >( function )();
	}
	catch ( const njson::exception& exception )
	{
		throw std::runtime_error( "\n-> Path: " + std::string( path ) +
		                          "\n-> Issue: " + std::string( exception.what() ) );
	}
}

template < class T >
[[nodiscard]] auto get( const njson& root, std::string_view path ) -> T
{
	return withPath( path,
	                 [ & ]() -> T
	                 {
		                 const njson* current = &root;

		                 std::size_t cursor = 0;
		                 while ( true )
		                 {
			                 const auto slashPos = path.find( '/', cursor );
			                 const auto keyView =
			                     path.substr( cursor, ( slashPos == std::string_view::npos ) ? ( path.size() - cursor )
			                                                                                 : ( slashPos - cursor ) );

			                 const std::string key( keyView );
			                 current = &current->at( key.c_str() );

			                 if ( slashPos == std::string_view::npos ) break;
			                 cursor = slashPos + 1;
		                 }

		                 return current->get< T >();
	                 } );
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
		json = njson::parse( file, /* callback */ nullptr, /* allowExceptions */ true, /* allowComments */ true );
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
		    .tempOptimal = get< float >( json, "vegetation/speed/tempOptimal" ),
		    .tempWidth   = get< float >( json, "vegetation/speed/tempWidth" ),
		    .humOptimal  = get< float >( json, "vegetation/speed/humOptimal" ),
		    .humWidth    = get< float >( json, "vegetation/speed/humWidth" ),
		};

		// Vegetation - limit
		Preset::Vegetation::Limit limit{
		    .tempOptimal   = get< float >( json, "vegetation/limit/tempOptimal" ),
		    .tempWidth     = get< float >( json, "vegetation/limit/tempWidth" ),
		    .humOptimal    = get< float >( json, "vegetation/limit/humOptimal" ),
		    .humWidth      = get< float >( json, "vegetation/limit/humWidth" ),
		    .elevHalf      = get< float >( json, "vegetation/limit/elevHalf" ),
		    .elevSteepness = get< float >( json, "vegetation/limit/elevSteepness" ),
		};

		return Preset{
		    .vegetation{ .speed = speed, .limit = limit },
		    .presetName        = path.filename().string(),
		    .gridDirectoryPath = gridPath,
		    .iterationTarget   = iterationTarget,
		    .rngSeed           = rngSeed,
		};
	}
	catch ( const std::runtime_error& exception )
	{
		return std::unexpected( "-> Parsing error" + std::string( exception.what() ) );
	}
	catch ( const std::exception& exception )
	{
		return std::unexpected( "-> Parsing error\n-> Issue: " + std::string( exception.what() ) );
	}
}
}  // namespace cc::app