#pragma once

#include <cstddef>

namespace cc::app
{
struct TickDataCollection
{
	std::size_t births{};
	std::size_t starvations{};
	std::size_t oldAgeDeaths{};
	float vegetationEaten{};
	float fleshEaten{};
};
}  // namespace cc::app