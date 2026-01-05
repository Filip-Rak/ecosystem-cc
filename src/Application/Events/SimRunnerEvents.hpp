#pragma once

#include <cstddef>

namespace cc::app::event
{
struct ResetSim
{};

struct ReachedTargetIteration
{};

struct Extinction
{
	std::size_t iteration;
};
}  // namespace cc::app::event