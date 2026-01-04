#include "Application/Utility/ReadGrid.hpp"

#include <cassert>
#include <cstddef>
#include <expected>
#include <format>
#include <optional>
#include <vector>

#include <entt/entt.hpp>
#include <glm/vec2.hpp>
#include <stb_image.h>

#include "Application/Constants/CellConstants.hpp"
#include "Application/Constants/FilePathConstants.hpp"
#include "Error.hpp"

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

using ReadingResult = std::expected< Layer, Error >;

constexpr std::size_t grayscaleChannel = 1;
constexpr float grayscaleRange         = 1.f;

[[nodiscard]] auto layerReadError( const std::filesystem::path& path, const Error& reason ) -> Error
{
	const auto absolutePath = std::filesystem::absolute( path ).string();
	const auto errorMessage = std::format( "-> Affected file: {}\n-> Issue: {}", absolutePath, reason );

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
auto readGridFromDirectory( entt::registry& registry, const std::filesystem::path& path )
    -> std::expected< Grid::Args, Error >
{
	constexpr const auto& cellConst = constant::cell;
	constexpr const auto& pathConst = constant::filePaths;

	// clang-format off
	const auto temperatureLayer =
	    readGridLayer( 
			path / pathConst.temperaturePath, 
			cellConst.temperatureRange, 
			cellConst.minTemperature
		);
	if ( !temperatureLayer )
	{
		return std::unexpected(temperatureLayer.error());
	}

	const glm::ivec2 validDimensions = { temperatureLayer->width, temperatureLayer->height };

	const auto elevationLayer =
	    readGridLayer( 
			path / pathConst.elevationPath, 
			cellConst.elevationRange, 
			cellConst.minElevation,
			validDimensions
		);
	if ( !elevationLayer )
	{
		return std::unexpected(elevationLayer.error());
	}

	const auto humidityLayer =
	    readGridLayer( 
			path / pathConst.humidityPath, 
			cellConst.humidityRange, 
			cellConst.minHumidity,
			validDimensions
		);
	if ( !humidityLayer )
	{
		return std::unexpected(humidityLayer.error());
	}
	// clang-format on

	Grid::Args args{ .registry          = registry,
	                 .width             = static_cast< uint16_t >( validDimensions.x ),
	                 .height            = static_cast< uint16_t >( validDimensions.y ),
	                 .temperatureValues = temperatureLayer->values,
	                 .humidityValues    = humidityLayer->values,
	                 .elevationValues   = elevationLayer->values };
	return args;
}
}  // namespace cc::app