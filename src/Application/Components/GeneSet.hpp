#pragma once

#include <cstdint>

namespace cc::app
{
struct Genes
{
	std::uint8_t perception;
	int refractoryPeriod;
	int lifespan;
	float maxEnergy;
	float temperaturePreference;
	float humidityPreference;
	float elevationPreference;
};
}  // namespace cc::app

namespace cc::app::component
{
struct GeneSet
{
	const Genes agentGenes;
	Genes futureGenes;
};
}  // namespace cc::app::component