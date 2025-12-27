#include "Application/System/Sim/AgentOffspringSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/OffspringIntent.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/Utility/AgentHelpers.hpp"

namespace cc::app
{
AgentOffspringSystem::AgentOffspringSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Grid >() );
}

auto AgentOffspringSystem::update() -> void
{
	auto& grid = m_registry.ctx().get< Grid >();

	const auto view = m_registry.view< const component::OffspringIntent, const component::GeneSet,
	                                   const component::Position, component::Vitals >();
	for ( const auto [ entity, geneSet, position, vitals ] : view.each() )
	{
		constexpr float mutationOffset = 0.5f;
		const auto newGenes            = mutateGenes( geneSet.futureGenes, mutationOffset );
		const auto childEntity         = createAgent( m_registry, newGenes );
		grid.addToSpatialGrid( entity, position.cellIndex );

		const auto& parentGenes          = geneSet.agentGenes;
		vitals.remainingRefractoryPeriod = parentGenes.refractoryPeriod;

		const float satietyCost = parentGenes.maxSatiety / 2.f;
		vitals.satiety -= satietyCost;
	}

	m_registry.remove< component::OffspringIntent >( view.begin(), view.end() );
}
}  // namespace cc::app