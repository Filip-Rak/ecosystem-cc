#pragma once

#include <glm/vec2.hpp>

#include "Engine/Interface/IService.hpp"

namespace cc
{
struct Transform;
struct ShapeCircle;

struct IRenderService : IFrameService
{
	auto beginFrame( entt::registry& registry ) -> void override = 0;
	auto endFrame( entt::registry& registry ) -> void override = 0;
};
};  // namespace cc