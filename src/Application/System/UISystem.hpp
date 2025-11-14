#pragma once

#include <entt/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class UISystem : public ISystem
{
public:
	UISystem( entt::registry& registry );

	auto update() -> void override;
};
}  // namespace cc::app