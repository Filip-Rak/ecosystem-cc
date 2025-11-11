#include "Application/Utility/ReadGrid.hpp"

#include <cstddef>
#include <expected>
#include <format>
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

const std::filesystem::path TemperaturePath = "temperature.png";
const std::filesystem::path ElevationPath = "elevation.png";
const std::filesystem::path HumidityPath = "humidity.png";

constexpr std::size_t GrayscaleChannel = 1;
constexpr float GrayscaleRange = 1.f;

[[nodiscard]] auto layerReadError( const std::filesystem::path& path, const ReadError& reason ) -> ReadError
{
	const ReadError absolutePath = std::filesystem::absolute( path ).string();
	const ReadError errorMessage = std::format( "-> Affected file: {}\n-> Issue: {}", absolutePath, reason );

	return errorMessage;
}

[[nodiscard]] auto readGridLayer( const std::filesystem::path& path, float mappingRange, float mappingMin,
                                  std::optional< glm::ivec2 > validDimensions = std::nullopt ) -> ReadingResult
{
	Layer layer;
	auto* const data = stbi_loadf( path.string().c_str(), &layer.width, &layer.height, nullptr, GrayscaleChannel );

	if ( data == nullptr )
	{
		return std::unexpected( layerReadError( path, stbi_failure_reason() ) );
	}
	if ( validDimensions && ( layer.width != validDimensions->x || layer.height != validDimensions->y ) )
	{
		std::string reason = std::format( "Invalid layer dimensions: {{ {}, {} }} should be {{ {}, {} }}", layer.width,
		                                  layer.height, validDimensions->x, validDimensions->y );
		return std::unexpected( layerReadError( path, reason ) );
	}

	const auto size = static_cast< std::size_t >( layer.width ) * static_cast< std::size_t >( layer.height );
	for ( std::size_t index = 0; index < size; index++ )
	{
		const float intensity = data[ index ];
		const float propertyValue = ( ( GrayscaleRange - intensity ) * mappingRange ) + mappingMin;
		layer.values.emplace_back( propertyValue );
	}

	stbi_image_free( data );
	return layer;
}
}  // namespace
auto readGridFromDirectory( entt::registry& registry, const std::filesystem::path& path ) -> std::optional< ReadError >
{
	constexpr const auto& Constant = constant::Cell;

	// clang-format off
	const auto temperatureLayer =
	    readGridLayer( 
			path / TemperaturePath, 
			Constant.TemperatureRange, 
			Constant.MinTemperature
		);
	if ( !temperatureLayer )
	{
		return temperatureLayer.error();
	}

	const glm::ivec2 validDimensions = { temperatureLayer->width, temperatureLayer->height };

	const auto elevationLayer =
	    readGridLayer( 
			path / ElevationPath, 
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
			path / HumidityPath, 
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
	const auto size = static_cast< const std::size_t >( grid.Height * grid.Width );

	for ( std::size_t index = 0; index < size; index++ )
	{
		const float cellTemperature = temperatureLayer->values[ index ];
		const float cellElevation = elevationLayer->values[ index ];
		const float cellHumidity = humidityLayer->values[ index ];

		grid.cells.emplace_back( 0.f, cellTemperature, cellElevation, cellHumidity );
	}

	return std::nullopt;
}
}  // namespace cc::app