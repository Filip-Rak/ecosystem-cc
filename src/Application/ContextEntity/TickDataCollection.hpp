#pragma once

#include <cstddef>

namespace cc::app
{
struct TickDataCollection
{
	std::size_t births{};
	std::size_t starvations{};
	std::size_t oldAgeDeaths{};
	std::size_t moves{};
	float vegetationEaten{};
	float fleshEaten{};
};
}  // namespace cc::app