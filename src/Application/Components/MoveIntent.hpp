#pragma once

#include <cstddef>

namespace cc::app::component
{
struct MoveIntent
{
	std::size_t cellIndex;
	float movemementCost;
};
}  // namespace cc::app::component