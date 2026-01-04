#pragma once

#include <cstddef>

namespace cc::app
{
struct GuiLog
{
	std::size_t totalBirths{};
	std::size_t totalStarvations{};
	std::size_t currentPopulation{};
	std::size_t highestPopulation{};
	std::size_t oldAgeDeaths{};
	float averageRefractoryPeriod{};
	float averageEnergyGene{};
	float averageAdaptation{};
	float averageFoodPref{};
	float totalVegetationEaten{};
};
}  // namespace cc::app