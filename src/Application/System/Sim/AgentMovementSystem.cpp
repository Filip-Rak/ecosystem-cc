#include "Application/System/Sim/AgentMovementSystem.hpp"

#include <cassert>
#include <entt/entt.hpp>

#include "Application/Components/NextMove.hpp"
#include "Application/Components/Position.hpp"
#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
AgentMovementSystem::AgentMovementSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Grid >() );
}

auto AgentMovementSystem::update() -> void
{
	auto& grid = m_registry.ctx().get< Grid >();
	auto view  = m_registry.view< const component::NextMove, component::Position >();

	for ( const auto& [ entity, nextMove, position ] : view.each() )
	{
		grid.moveEntity( entity, position.cellIndex, nextMove.targetCell );
		position.cellIndex = nextMove.targetCell;
	}

	m_registry.remove< component::NextMove >( view.begin(), view.end() );
}
}  // namespace cc::app