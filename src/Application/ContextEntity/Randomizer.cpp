#include "Application/ContextEntity/Randomizer.hpp"

#include <algorithm>

#include "Application/Components/GeneSet.hpp"
#include "Application/ContextEntity/Preset.hpp"

#include <entt/entity/fwd.hpp>

namespace cc::app
{
Randomizer::Randomizer( const Preset& preset ) : m_preset( preset ), m_generator( preset.rngSeed ) {}

auto Randomizer::mutateGenes( const Genes& baseGenes, float offset ) -> Genes
{
	std::uniform_real_distribution< float > dist( -offset, offset );
	auto mutateNormalized = [ & ]( float& value ) -> void
	{ value = std::clamp( value + dist( m_generator ), 0.0f, 1.0f ); };

	Genes newGenes = baseGenes;
	mutateNormalized( newGenes.temperaturePreference );
	mutateNormalized( newGenes.humidityPreference );
	mutateNormalized( newGenes.elevationPreference );

	newGenes.maxEnergy = std::max( 0.0f, newGenes.maxEnergy + dist( m_generator ) );

	const auto refractorMutation = m_preset.agent.modifier.refractoryMutationRate;
	std::uniform_int_distribution< int > signDist( -refractorMutation, refractorMutation );
	newGenes.refractoryPeriod = std::max( 1, newGenes.refractoryPeriod + signDist( m_generator ) );

	return newGenes;
}

auto Randomizer::reset() -> void
{
	m_generator.seed( m_preset.rngSeed );
}
}  // namespace cc::app