#pragma once

#include <entt/entity/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class AgentPassingSystem : public ISystem
{
public:
	AgentPassingSystem( entt::registry& registry );
	auto update() -> void override;

private:
	entt::registry& m_registry;
};
}  // namespace cc::app