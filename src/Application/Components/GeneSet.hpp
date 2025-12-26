#pragma once

#include <cstddef>

namespace cc::app
{
struct Genes
{
	std::size_t perception;
	float maxSatiety;
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