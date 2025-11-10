#include "Application/Utility/ReadGrid.hpp"

#include <expected>
#include <filesystem>  // TODO: Use instead of strings
#include <string_view>
#include <vector>

#include <entt/entt.hpp>
#include <glm/vec2.hpp>
#include <stb_image.h>

#include "Application/Constants/CellConstants.hpp"
#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
namespace
{
struct Layer
{
	int width;
	int height;
	std::vector< float > values;
};

using ReadingResult = std::expected< Layer, ReadError >;

constexpr std::string_view TemperatureFilename = "temperature.png";
constexpr std::string_view ElevationFilename = "elevation.png";
constexpr std::string_view HumidityFilename = "humidity.png";

constexpr std::size_t GrayscaleChannel = 1;

[[nodiscard]] auto readGridLayer( const std::string& path, float mappingRange, float mappingMin,
                                  std::optional< glm::ivec2 > validDimensions = std::nullopt ) -> ReadingResult
{
	Layer layer;
	std::filesystem::path absolutePath = std::filesystem::absolute( path );
	auto* const data = stbi_loadf( path.c_str(), &layer.width, &layer.height, nullptr, GrayscaleChannel );

	if ( data == nullptr )
	{
		std::string reason = stbi_failure_reason();
		std::string errorMsg = absolutePath.string() + "': " + reason;
		return std::unexpected( stbi_failure_reason() + errorMsg );
	}
	if ( validDimensions && ( layer.height != validDimensions->x || layer.width != validDimensions->y ) )
	{
		return std::unexpected( "Invalid layer dimensions" );
	}

	const auto size = static_cast< std::size_t >( layer.width ) * static_cast< std::size_t >( layer.height );
	for ( std::size_t index = 0; index < size; index++ )
	{
		const float intensity = data[ index ];

		const float propertyValue = ( intensity * mappingRange ) + mappingMin;
		layer.values.emplace_back( propertyValue );
	}

	stbi_image_free( data );
	return layer;
}
}  // namespace
auto readGridFromDirectory( entt::registry& registry, const std::string& path ) -> std::optional< ReadError >
{
	constexpr const auto& Constant = constant::Cell;

	// clang-format off
	const auto temperatureLayer =
	    readGridLayer( 
			path + TemperatureFilename.data(), 
			Constant.TemperatureRange, 
			Constant.MinTemperature
		);
	if ( !temperatureLayer )
	{
		return temperatureLayer.error();
	}

	const glm::ivec2 validDimensions = { temperatureLayer->height, temperatureLayer->width };

	const auto elevationLayer =
	    readGridLayer( 
			path + ElevationFilename.data(), 
			Constant.ElevationRange, 
			Constant.MinElevation,
			validDimensions
		);
	if ( !elevationLayer )
	{
		return elevationLayer.error();
	}

	const auto humidityLayer =
	    readGridLayer( 
			path + HumidityFilename.data(), 
			Constant.HumidityRange, 
			Constant.MinHumidity,
			validDimensions
		);
	if ( !humidityLayer )
	{
		return humidityLayer.error();
	}
	// clang-format on

	auto& grid = registry.ctx().emplace< Grid >( validDimensions.x, validDimensions.y );
	for ( std::size_t index = 0; index < grid.cells.capacity(); index++ )
	{
		const float cellTemperature = temperatureLayer->values[ index ];
		const float cellElevation = elevationLayer->values[ index ];
		const float cellHumidity = humidityLayer->values[ index ];

		grid.cells.emplace_back( 0.f, cellTemperature, cellElevation, cellHumidity );
	}

	return std::nullopt;
}
}  // namespace cc::app