#include "Application/Utility/ReadGrid.hpp"

#include <expected>
#include <string_view>
#include <vector>

#include <entt/entt.hpp>
#include <stb_image.h>

#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
namespace
{
struct LayerMap
{
	const uint16_t width;
	const uint16_t height;
	std::vector< float > values;
};

using ReadingResult = std::expected< LayerMap, ReadError >;
using ValidationResult = std::optional< ReadError >;

constexpr std::string_view TemperatureFilename = "temperature.png";
constexpr std::string_view ElevationFilename = "elevation.png";
constexpr std::string_view HumidityFilename = "humidity.png";

[[nodiscard]] auto readTemperatureMap( const std::string& /*path*/ ) -> ReadingResult
{
	return LayerMap{};
}

[[nodiscard]] auto readElevationMap( const std::string& /*path*/ ) -> ReadingResult
{
	return LayerMap{};
}

[[nodiscard]] auto readHumidityMap( const std::string& /*path*/ ) -> ReadingResult
{
	return LayerMap{};
}

[[nodiscard]] auto invalidRead( const uint16_t validWidth, const uint16_t validHeight, const ReadingResult& read )
    -> ValidationResult
{
	if ( !read ) return read.error();
	if ( read->width != validWidth || read->height != validHeight ) return std::string( "Layer dimension mismatch" );

	return std::nullopt;
}
}  // namespace
auto readGridFromDirectory( entt::registry& registry, const std::string& path ) -> ValidationResult
{
	const auto temperatureLayer = readTemperatureMap( path + TemperatureFilename.data() );
	if ( !temperatureLayer ) return temperatureLayer.error();

	const uint16_t validWidth = temperatureLayer->width;
	const uint16_t validHeight = temperatureLayer->height;

	const auto elevationLayer = readElevationMap( path + ElevationFilename.data() );
	if ( auto isInvalid = invalidRead( validWidth, validHeight, elevationLayer ); isInvalid ) return isInvalid;

	const auto humidityLayer = readHumidityMap( path + HumidityFilename.data() );
	if ( auto isInvalid = invalidRead( validWidth, validHeight, humidityLayer ); isInvalid ) return isInvalid;

	auto& grid = registry.ctx().emplace< Grid >( validWidth, validHeight );
	for ( std::size_t index = 0; index < grid.cells.size(); index++ )
	{
		const auto cellTemperature = temperatureLayer->values[ index ];
		const auto cellElevation = elevationLayer->values[ index ];
		const auto cellHumidity = humidityLayer->values[ index ];

		grid.cells.emplace_back( 0.f, cellTemperature, cellElevation, cellHumidity );
	}

	return std::nullopt;
}
}  // namespace cc::app