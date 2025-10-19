#pragma once

#include "Engine/Interface/ISystem.hpp"

namespace cc::eco
{
class InputSystem : public ISystem
{
public:
	InputSystem( entt::registry& registry );
	auto update( entt::registry& registry ) -> void override;
};
}  // namespace cc::eco