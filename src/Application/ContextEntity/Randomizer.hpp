#pragma once

#include <cstdint>
#include <random>

namespace cc::app
{
struct Genes;

class Randomizer
{
public:
	explicit Randomizer( std::uint32_t seed );
	[[nodiscard]] auto mutateGenes( const Genes& baseGenes, float offset ) -> Genes;
	auto reset() -> void;

private:
	const std::uint32_t m_seed;
	std::mt19937 m_generator;
};
}  // namespace cc::app