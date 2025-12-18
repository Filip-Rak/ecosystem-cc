#pragma once

#include <cstddef>

namespace cc::app
{
namespace constant
{
constexpr int defaultSpeed = 60;
}

struct SimRunnerData
{
	std::size_t iteration = 0;

	int speed          = constant::defaultSpeed;
	bool paused        = true;
	bool targetReached = false;
};
}  // namespace cc::app