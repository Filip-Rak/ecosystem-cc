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
	bool paused = true;
	int speed = constant::DefaultSpeed;
	std::size_t iteration = 0;
};
}  // namespace cc::app