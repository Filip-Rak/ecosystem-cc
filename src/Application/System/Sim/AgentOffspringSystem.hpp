#pragma once

#include <entt/entity/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class AgentOffspringSystem : public ISystem
{
public:
	AgentOffspringSystem( entt::registry& m_registry );
	auto update() -> void override;

private:
	entt::registry& m_registry;
};
}  // namespace cc::app