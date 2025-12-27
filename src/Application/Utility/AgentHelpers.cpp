#include "Application/Utility/AgentHelpers.hpp"

#include <random>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"

namespace cc::app
{
auto createAgent( entt::registry& registry, const Genes& genes ) -> entt::entity
{
	const auto entity = registry.create();
	registry.emplace< component::GeneSet >( entity, genes, genes );
	registry.emplace< component::Vitals >( entity, genes.maxSatiety, genes.refractoryPeriod );
	registry.emplace< component::Position >( entity );

	return entity;
}

auto mutateGenes( const Genes& baseGenes, float mutationOffset ) -> Genes
{
	static std::random_device rd;
	static std::mt19937 gen( rd() );

	std::uniform_real_distribution< float > dist( -mutationOffset, mutationOffset );

	Genes newGenes = baseGenes;
	auto mutate    = [ & ]( float& value ) -> void { value = std::clamp( value + dist( gen ), 0.0f, 1.0f ); };

	mutate( newGenes.maxSatiety );
	mutate( newGenes.temperaturePreference );
	mutate( newGenes.humidityPreference );
	mutate( newGenes.elevationPreference );

	return newGenes;
}
}  // namespace cc::app