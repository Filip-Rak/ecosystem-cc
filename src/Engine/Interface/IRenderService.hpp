#pragma once

#include <vector>

#include <glm/fwd.hpp>

#include "Engine/Interface/IService.hpp"
#include "Engine/Utility/Color.hpp"
#include "Engine/Utility/RenderHandles.hpp"

namespace cc
{
struct IRenderService : IFrameService
{
	auto beginFrame() -> void override = 0;
	auto endFrame() -> void override = 0;

	[[nodiscard]] auto virtual createCamera() -> CameraHandle = 0;
	auto virtual setPosition( CameraHandle handle, glm::vec2 position ) -> void = 0;
	auto virtual setZoom( CameraHandle handle, float level ) -> void = 0;
	auto virtual setActiveCamera( CameraHandle handle ) -> void = 0;

	[[nodiscard]] auto virtual createGrid( std::size_t width, std::size_t height, glm::vec2 position, float cellSize )
	    -> GridHandle = 0;
	auto virtual setGridColors( GridHandle& handle, const std::vector< Color >& colors ) -> void = 0;

	auto virtual draw( GridHandle handle ) -> void = 0;
};
};  // namespace cc