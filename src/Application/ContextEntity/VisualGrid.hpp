#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include <glm/vec2.hpp>

#include "Application/ContextEntity/VisMode.hpp"
#include "Engine/Utility/Color.hpp"

namespace cc::app
{
struct VisualGrid
{
	const uint16_t width;
	const uint16_t height;

	VisModeEnum visMode = VisModeEnum::Vegetation;
	std::optional< glm::ivec2 > cellPositionUnderMouse;
	std::vector< Color > colors;

	VisualGrid( uint16_t width, uint16_t height )
	    : width( width ), height( height ), colors( static_cast< std::size_t >( width * height ) ) {};
};
}  // namespace cc::app