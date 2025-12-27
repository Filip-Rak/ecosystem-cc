#pragma once

#include <cstddef>

namespace cc::app::component
{
struct Vitals
{
	float satiety;
	std::size_t remainingRefractoryPeriod;
};
}  // namespace cc::app::component