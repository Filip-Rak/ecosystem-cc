#pragma once

#include <glm/fwd.hpp>

#include "Engine/Interface/IService.hpp"

namespace cc
{
struct GridHandle;

struct IRenderService : IFrameService
{
	auto beginFrame( entt::registry& registry ) -> void override = 0;
	auto endFrame( entt::registry& registry ) -> void override = 0;

	auto virtual createGrid( std::size_t width, std::size_t height, glm::vec2 position,
	                         float cellSize ) -> GridHandle = 0;
	auto virtual draw( GridHandle handle ) -> void = 0;
};
};  // namespace cc