#pragma once

#include <cstddef>
#include <vector>

#include "Application/ContextEntity/VisMode.hpp"
#include "Engine/Utility/Color.hpp"

namespace cc::app
{
struct VisualGrid
{
	VisModeEnum visMode = VisModeEnum::Vegetation;
	std::vector< Color > colors;

	VisualGrid( std::size_t size ) : colors( size ) {};
};
}  // namespace cc::app