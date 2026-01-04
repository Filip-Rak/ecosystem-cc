#include "Application/Utility/AgentHelpers.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/TickDataCollection.hpp"

namespace cc::app
{
auto createAgent( entt::registry& registry, const Genes& genes, float energy ) -> entt::entity
{
	assert( registry.ctx().contains< TickDataCollection >() );
	auto& tickData = registry.ctx().get< TickDataCollection >();

	tickData.births++;

	const auto entity = registry.create();
	registry.emplace< component::GeneSet >( entity, genes, genes );
	registry.emplace< component::Vitals >( entity, energy, genes.refractoryPeriod, 0 );
	registry.emplace< component::Position >( entity );

	return entity;
}
}  // namespace cc::app