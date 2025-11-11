#pragma once

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class VegetationSystem : public ISystem
{
public:
	VegetationSystem( entt::registry& registry );
	auto update( entt::registry& registry ) -> void override;
};
}  // namespace cc::app