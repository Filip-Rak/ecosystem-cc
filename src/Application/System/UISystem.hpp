#pragma once

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class UISystem : public ISystem
{
public:
	UISystem( entt::registry& registry );

	auto update( entt::registry& registry ) -> void override;
};
}  // namespace cc::app