#pragma once

#include <cstddef>

namespace cc::app::component
{
struct MoveIntent
{
	std::size_t targetCell;
};
}  // namespace cc::app::component