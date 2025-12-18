#include "Application/Utility/ReadGrid.hpp"

#include <cassert>
#include <cstddef>
#include <expected>
#include <format>
#include <vector>

#include <entt/entt.hpp>
#include <glm/vec2.hpp>
#include <stb_image.h>

#include "Application/Constants/CellConstants.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Preset.hpp"

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

const std::filesystem::path temperaturePath = "temperature.png";
const std::filesystem::path elevationPath   = "elevation.png";
const std::filesystem::path humidityPath    = "humidity.png";

constexpr std::size_t grayscaleChannel = 1;
constexpr float grayscaleRange         = 1.f;

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
	auto* const data = stbi_loadf( path.string().c_str(), &layer.width, &layer.height, nullptr, grayscaleChannel );

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
		const float intensity     = data[ index ];
		const float propertyValue = ( ( grayscaleRange - intensity ) * mappingRange ) + mappingMin;
		layer.values.emplace_back( propertyValue );
	}

	stbi_image_free( data );
	return layer;
}
}  // namespace
auto readGridFromDirectory( entt::registry& registry, const std::filesystem::path& path ) -> std::optional< ReadError >
{
	constexpr const auto& constant = constant::cell;

	// clang-format off
	const auto temperatureLayer =
	    readGridLayer( 
			path / temperaturePath, 
			constant.temperatureRange, 
			constant.minTemperature
		);
	if ( !temperatureLayer )
	{
		return temperatureLayer.error();
	}

	const glm::ivec2 validDimensions = { temperatureLayer->width, temperatureLayer->height };

	const auto elevationLayer =
	    readGridLayer( 
			path / elevationPath, 
			constant.elevationRange, 
			constant.minElevation,
			validDimensions
		);
	if ( !elevationLayer )
	{
		return elevationLayer.error();
	}

	const auto humidityLayer =
	    readGridLayer( 
			path / humidityPath, 
			constant.humidityRange, 
			constant.minHumidity,
			validDimensions
		);
	if ( !humidityLayer )
	{
		return humidityLayer.error();
	}
	// clang-format on

	auto& grid = registry.ctx().emplace< Grid >( registry, validDimensions.x, validDimensions.y );

	assert( registry.ctx().contains< Preset >() );
	const Preset::Vegetation& vegetationPreset = registry.ctx().get< Preset >().vegetation;

	const auto size = static_cast< const std::size_t >( grid.height * grid.width );

	for ( std::size_t index = 0; index < size; index++ )
	{
		const float cellTemperature = temperatureLayer->values[ index ];
		const float cellElevation   = elevationLayer->values[ index ];
		const float cellHumidity    = humidityLayer->values[ index ];

		grid.cells.emplace_back( 0.f, cellTemperature, cellElevation, cellHumidity, vegetationPreset );
	}

	return std::nullopt;
}
}  // namespace cc::app