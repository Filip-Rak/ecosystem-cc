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
	const uint16_t Width;
	const uint16_t Height;

	VisModeEnum visMode = VisModeEnum::Vegetation;
	std::optional< glm::ivec2 > cellPositionUnderMouse;
	std::vector< Color > colors;

	VisualGrid( uint16_t width, uint16_t height, std::size_t size )
	    : Width( width ), Height( height ), colors( size ) {};
};
}  // namespace cc::app