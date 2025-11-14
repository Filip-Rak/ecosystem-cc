#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include <glm/vec2.hpp>

#include "Application/ContextEntity/VisMode.hpp"
#include "Engine/Utility/Color.hpp"

namespace cc::app
{
struct VisualGrid
{
	VisModeEnum visMode = VisModeEnum::Vegetation;
	std::optional< glm::ivec2 > cellPositionUnderMouse;
	std::vector< Color > colors;

	VisualGrid( std::size_t size ) : colors( size ) {};
};
}  // namespace cc::app