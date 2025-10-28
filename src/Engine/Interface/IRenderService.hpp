#pragma once

#include <glm/fwd.hpp>

#include "Engine/Interface/IService.hpp"
#include "Engine/Utility/RenderHandles.hpp"  // TODO: Should this header be exposed here?

namespace cc
{
struct IRenderService : IFrameService
{
	auto beginFrame( entt::registry& registry ) -> void override = 0;
	auto endFrame( entt::registry& registry ) -> void override = 0;

	auto virtual createCamera() -> CameraHandle = 0;  // Figure out a more general method
	auto virtual moveCamera( CameraHandle handle, glm::vec2 offset ) -> void = 0;
	auto virtual zoomCamera( CameraHandle handle, float delta ) -> void = 0;
	auto virtual setCamera( CameraHandle handle ) -> void = 0;

	auto virtual createGrid( std::size_t width, std::size_t height, glm::vec2 position,
	                         float cellSize ) -> GridHandle = 0;

	auto virtual draw( GridHandle handle ) -> void = 0;
};
};  // namespace cc