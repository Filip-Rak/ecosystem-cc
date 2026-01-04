#pragma once

#include "Engine/Interface/ISystem.hpp"

#include <entt/entity/fwd.hpp>

namespace cc::app
{
class TickLogSystem : public ISystem
{
public:
	TickLogSystem( entt::registry& registry );
	auto update() -> void override;

private:
	entt::registry& m_registry;
};
}  // namespace cc::app