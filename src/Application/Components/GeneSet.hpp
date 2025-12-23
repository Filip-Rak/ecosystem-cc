#pragma once

#include <cstddef>

namespace cc::app::component
{
struct GeneSet
{
	struct Genes
	{
		float maxEnergy        = 1.f;
		std::size_t perception = 2uz;
	};

	const Genes agentGenes;
	Genes futureGenes;
};
}  // namespace cc::app::component