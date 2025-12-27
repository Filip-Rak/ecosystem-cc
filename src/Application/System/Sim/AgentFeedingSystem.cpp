#include "Application/System/Sim/AgentFeedingSystem.hpp"

#include <algorithm>
#include <cassert>
#include <entt/entt.hpp>

#include "Application/Components/EatIntent.hpp"
#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
AgentFeedingSystem::AgentFeedingSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( m_registry.ctx().contains< Grid >() );
	assert( m_registry.ctx().contains< Preset >() );
}

auto AgentFeedingSystem::update() -> void
{
	const auto maxIntake = m_registry.ctx().get< Preset >().agent.modifier.maxIntake;
	auto& grid           = m_registry.ctx().get< Grid >();
	auto& cells          = grid.cells();

	const auto view = m_registry.view< const component::EatIntent, const component::Position, const component::GeneSet,
	                                   component::Vitals >();
	for ( const auto [ entity, position, geneSet, vitals ] : view.each() )
	{
		auto& vegetation   = cells[ position.cellIndex ].vegetation;
		const float hunger = geneSet.agentGenes.maxSatiety - vitals.satiety;

		const float eaten = std::min( { maxIntake, vegetation, hunger } );
		vegetation -= eaten;
		vitals.satiety += eaten;
	}

	m_registry.remove< component::EatIntent >( view.begin(), view.end() );
}
}  // namespace cc::app