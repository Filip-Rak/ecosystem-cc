#pragma once

#include <cstddef>
#include <vector>

#include "Engine/Utility/Color.hpp"

namespace cc::app
{
struct VisualGrid
{
	// TODO: Maybe try array instead?
	std::vector< Color > colors;
	VisualGrid( std::size_t size ) : colors( size ) {};
};
}  // namespace cc::app