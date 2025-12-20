#pragma once

#include <cstddef>

namespace cc::app::component
{
struct Position
{
	std::size_t cellIndex;
	std::size_t spatialIndex;
};
}  // namespace cc::app::component