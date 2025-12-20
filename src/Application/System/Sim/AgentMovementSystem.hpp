#pragma once

#include "Engine/Interface/ISystem.hpp"
#include "entt/entity/fwd.hpp"

namespace cc::app
{
class AgentMovementSystem : public ISystem
{
public:
	AgentMovementSystem( entt::registry& registry );
	auto update() -> void override;

private:
	entt::registry& m_registry;
};
}  // namespace cc::app