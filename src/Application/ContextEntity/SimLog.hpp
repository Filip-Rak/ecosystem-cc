#pragma once

#include <cstddef>

namespace cc::app
{
struct SimLog
{
	std::size_t totalBirths{};
	std::size_t highestPopulation{};
	std::size_t currentPopulation{};
	std::size_t starvations{};
	std::size_t oldAgeDeaths{};
};
}  // namespace cc::app