#pragma once

namespace cc::app::component
{
struct Vitals
{
	float energy;
	int remainingRefractoryPeriod;
	int lifespanLeft;
};
}  // namespace cc::app::component