#include "Application/System/Sim/AgentOffspringSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/OffspringIntent.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/Randomizer.hpp"
#include "Application/Utility/AgentHelpers.hpp"

namespace cc::app
{
AgentOffspringSystem::AgentOffspringSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Grid >() );
	assert( registry.ctx().contains< Preset >() );
	assert( registry.ctx().contains< Randomizer >() );

	const auto prealloc = 100uz;
	m_agentsToCreate.reserve( prealloc );
}

auto AgentOffspringSystem::update() -> void
{
	auto& random = m_registry.ctx().get< Randomizer >();
	auto& grid   = m_registry.ctx().get< Grid >();
	m_agentsToCreate.clear();

	const auto view = m_registry.view< const component::OffspringIntent, const component::GeneSet,
	                                   const component::Position, component::Vitals >();
	for ( const auto [ entity, geneSet, position, vitals ] : view.each() )
	{
		constexpr float half = 2.f;
		vitals.energy /= half;
		vitals.remainingRefractoryPeriod = geneSet.agentGenes.refractoryPeriod;
		m_agentsToCreate.emplace_back( geneSet.futureGenes, position.cellIndex, vitals.energy );
	}

	m_registry.remove< component::OffspringIntent >( view.begin(), view.end() );

	auto& preset               = m_registry.ctx().get< Preset >();
	const float mutationOffset = preset.agent.modifier.furtherMutations;

	for ( const auto& args : m_agentsToCreate )
	{
		const auto newGenes    = random.mutateGenes( args.parentFutureGenes, mutationOffset );
		const auto childEntity = createAgent( m_registry, newGenes, args.energy );
		grid.addToSpatialGrid( childEntity, args.position );
	}
}
}  // namespace cc::app