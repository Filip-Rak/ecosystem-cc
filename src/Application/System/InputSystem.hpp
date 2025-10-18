#pragma once

#include "Engine/Interface/ISystem.hpp"

namespace cc::eco
{
class InputSystem : public ISystem
{
	auto update( entt::registry& registry ) -> void override;
};
}  // namespace cc::eco