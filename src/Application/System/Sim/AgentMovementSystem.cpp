#include "Application/System/Sim/AgentMovementSystem.hpp"

#include <entt/entt.hpp>

#include "Application/Components/NextMove.hpp"

namespace cc::app
{
AgentMovementSystem::AgentMovementSystem( entt::registry& registry ) : m_registry( registry ) {}

auto AgentMovementSystem::update() -> void
{
	const auto view = m_registry.view< component::NextMove >();
	for ( const auto [ entity, targetCell ] : view.each() )
	{}
}
}  // namespace cc::app