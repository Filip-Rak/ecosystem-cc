#include "Application/System/Sim/AgentDecisionSystem.hpp"

#include <cassert>
#include <entt/entt.hpp>

namespace cc::app
{
AgentDecisionSystem::AgentDecisionSystem( entt::registry& registry ) : m_registry( registry ) {}

auto AgentDecisionSystem::update() -> void {}
}  // namespace cc::app