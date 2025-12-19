#pragma once

#include <cstddef>

namespace cc::app::component
{
struct Agent
{
	std::size_t cellIndex;
	float energy;
};
}  // namespace cc::app::component