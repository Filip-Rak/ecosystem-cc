#pragma once

#include <cstddef>

namespace cc::app::component
{
struct Genes
{
	float maxEnergy        = 100.f;
	std::size_t perception = 2uz;
};
}  // namespace cc::app::component