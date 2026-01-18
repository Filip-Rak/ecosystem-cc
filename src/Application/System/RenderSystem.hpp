#pragma once

#include <entt/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"
#include "Engine/Utility/RenderHandles.hpp"

namespace cc
{
struct IRenderService;
}

namespace cc::app
{
struct Camera;
struct Grid;
struct VisualGrid;

class RenderSystem : public ISystem
{
public:
	RenderSystem( entt::registry& registry );
	auto update() -> void override;

private:
	auto updateGridHandle( const Grid& grid, VisualGrid& visualGrid ) -> void;
	auto updateCameraHandle( const Camera& camera ) -> void;

	// Initialization order matters.
	entt::registry& m_registry;
	IRenderService& m_renderer;
	const GridHandle m_gridHandle;
	const CameraHandle m_cameraHandle;
};
}  // namespace cc::app