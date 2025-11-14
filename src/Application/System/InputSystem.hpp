#pragma once

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class InputSystem : public ISystem
{
public:
	InputSystem( entt::registry& registry );
	auto update() -> void override;

private:
	entt::registry& m_registry;
};
}  // namespace cc::app