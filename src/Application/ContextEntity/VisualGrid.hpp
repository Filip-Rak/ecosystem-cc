#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Engine/Utility/Color.hpp"

namespace cc::app
{
enum class VisMode : uint8_t
{
	Vegetation,
	Elevation,
	Humidity,
	Temperature,
};

struct VisualGrid
{
	VisMode visMode = VisMode::Vegetation;
	std::vector< Color > colors;

	VisualGrid( std::size_t size ) : colors( size ) {};
};
}  // namespace cc::app