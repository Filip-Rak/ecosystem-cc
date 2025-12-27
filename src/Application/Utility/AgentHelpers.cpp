#include "Application/Utility/AgentHelpers.hpp"

#include <random>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"

namespace cc::app
{
auto mutateGenes( const Genes& baseGenes, float mutationOffset ) -> Genes
{
	static std::random_device rd;
	static std::mt19937 gen( rd() );

	std::uniform_real_distribution< float > dist( -mutationOffset, mutationOffset );

	Genes newGenes = baseGenes;

	// Standard mutation for normalized genes (0.0 to 1.0)
	auto mutateClamped = [ & ]( float& value ) -> void { value = std::clamp( value + dist( gen ), 0.0f, 1.0f ); };

	mutateClamped( newGenes.temperaturePreference );
	mutateClamped( newGenes.humidityPreference );
	mutateClamped( newGenes.elevationPreference );

	// Uncapped mutation for satiety
	newGenes.maxSatiety = std::max( 0.0f, newGenes.maxSatiety + dist( gen ) );
	return newGenes;
}

auto createAgent( entt::registry& registry, const Genes& genes ) -> entt::entity
{
	const auto entity = registry.create();
	registry.emplace< component::GeneSet >( entity, genes, genes );
	registry.emplace< component::Vitals >( entity, genes.maxSatiety, genes.refractoryPeriod );
	registry.emplace< component::Position >( entity );

	return entity;
}
}  // namespace cc::app