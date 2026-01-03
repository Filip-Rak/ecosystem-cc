#pragma once

#include <random>

namespace cc::app
{
struct Preset;
struct Genes;

class Randomizer
{
public:
	explicit Randomizer( const Preset& preset );
	[[nodiscard]] auto mutateGenes( const Genes& baseGenes, float offset ) -> Genes;
	auto reset() -> void;

private:
	const Preset& m_preset;
	std::mt19937 m_generator;
};
}  // namespace cc::app