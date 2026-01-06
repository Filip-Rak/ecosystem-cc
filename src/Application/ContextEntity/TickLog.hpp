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
	std::size_t moves;
	std::size_t occupiedCells;
	float vegetationEaten;
	float vegetationTotal;
	float meanVegetation;
	float meanCrowd;
	float meanEnergy;
	float energyVar;
	float energyP50;
	float energyP90;
	float meanEnergyGene;
	float energyGeneVar;
	float energyGeneP50;
	float energyGeneP90;
	float meanTempAdaptation;
	float meanHumAdaptation;
	float meanElevAdaptation;
};
}  // namespace cc::app