#include "Application/ContextEntity/Randomizer.hpp"

#include "Application/Components/GeneSet.hpp"

namespace cc::app
{
Randomizer::Randomizer( std::uint32_t seed ) : m_seed( seed ), m_generator( seed ) {}

auto Randomizer::mutateGenes( const Genes& baseGenes, float offset ) -> Genes
{
	std::uniform_real_distribution< float > dist( -offset, offset );

	Genes newGenes = baseGenes;

	// Standard mutation for normalized genes (0.0 to 1.0)
	auto mutateClamped = [ & ]( float& value ) -> void
	{ value = std::clamp( value + dist( m_generator ), 0.0f, 1.0f ); };

	mutateClamped( newGenes.temperaturePreference );
	mutateClamped( newGenes.humidityPreference );
	mutateClamped( newGenes.elevationPreference );

	// Uncapped mutations
	newGenes.maxEnergy = std::max( 0.0f, newGenes.maxEnergy + dist( m_generator ) );
	return newGenes;
}

auto Randomizer::reset() -> void
{
	m_generator.seed( m_seed );
}
}  // namespace cc::app