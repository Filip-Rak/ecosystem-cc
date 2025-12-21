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
	auto& grid      = m_registry.ctx().get< Grid >();
	const auto view = m_registry.view< component::Position, const component::NextMove >();

	for ( const auto& [ entity, position, nextMove ] : view.each() )
	{
		grid.moveEntity( entity, position.cellIndex, nextMove.cellIndex );
	}

	m_registry.remove< component::NextMove >( view.begin(), view.end() );
}
}  // namespace cc::app