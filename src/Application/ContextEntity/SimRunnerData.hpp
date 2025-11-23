#pragma once

#include <cstddef>

namespace cc::app
{
namespace constant
{
constexpr int DefaultSpeed = 60;
}

struct SimRunnerData
{
	std::size_t iteration = 0;

	int speed = constant::DefaultSpeed;
	bool paused = true;
	bool targetReached = false;
};
}  // namespace cc::app