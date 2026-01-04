#pragma once

#include <cstddef>

namespace cc::app
{
struct TickLog
{
	std::size_t iteration;
	std::size_t liveAgents;
	std::size_t births;
	std::size_t starvations;
	std::size_t ageDeaths;
	float vegetationEaten;
	float meanEnergy;
	float meanEnergyGene;
	float meanTempAdaptation;
	float meanHumAdaptation;
	float meanElevAdaptation;
};
}  // namespace cc::app