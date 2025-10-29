#pragma once

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class CameraMovementSystem : public ISystem
{
public:
	CameraMovementSystem( entt::registry& registry );
	auto update( entt::registry& registry ) -> void override;
};
}  // namespace cc::app