#include "Application/System/Sim/AgentOffspringSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/OffspringIntent.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/Utility/AgentHelpers.hpp"

namespace cc::app
{
AgentOffspringSystem::AgentOffspringSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Grid >() );
	assert( registry.ctx().contains< Preset >() );
}

auto AgentOffspringSystem::update() -> void
{
	auto& grid   = m_registry.ctx().get< Grid >();
	auto& preset = m_registry.ctx().get< Preset >();

	const auto view = m_registry.view< const component::OffspringIntent, const component::GeneSet,
	                                   const component::Position, component::Vitals >();
	for ( const auto [ entity, geneSet, position, vitals ] : view.each() )
	{
		const float mutationOffset = preset.agent.modifier.furtherMutations;
		const auto newGenes        = mutateGenes( geneSet.futureGenes, mutationOffset );
		const auto childEntity     = createAgent( m_registry, newGenes );
		grid.addToSpatialGrid( childEntity, position.cellIndex );

		vitals.remainingRefractoryPeriod = geneSet.agentGenes.refractoryPeriod;

		constexpr float half = 2.f;
		vitals.energy /= half;
	}

	m_registry.remove< component::OffspringIntent >( view.begin(), view.end() );
}
}  // namespace cc::app