#include "Application/System/Sim/AgentMovementSystem.hpp"

#include <cassert>
#include <entt/entt.hpp>

#include "Application/Components/MoveIntent.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
AgentMovementSystem::AgentMovementSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Grid >() );
}

auto AgentMovementSystem::update() -> void
{
	auto& grid                = m_registry.ctx().get< Grid >();
	const auto& spatialGrid   = grid.getSpatialGrid();
	const auto& preset        = m_registry.ctx().get< Preset >();
	const auto cellAgentLimit = preset.agent.modifier.cellAgentLimit;

	const auto view = m_registry.view< const component::MoveIntent, const component::Position, component::Vitals >();
	for ( const auto& [ entity, moveIntent, position, vitals ] : view.each() )
	{
		if ( spatialGrid[ moveIntent.cellIndex ].size() >= cellAgentLimit ) continue;

		grid.moveEntity( entity, position.cellIndex, moveIntent.cellIndex );
		vitals.energy -= moveIntent.movemementCost;
	}

	m_registry.remove< component::MoveIntent >( view.begin(), view.end() );
}
}  // namespace cc::app