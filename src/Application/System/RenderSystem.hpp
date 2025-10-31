#pragma once

#include "Engine/Interface/IRenderService.hpp"
#include "Engine/Interface/ISystem.hpp"
#include "Engine/Utility/RenderHandles.hpp"

namespace cc::app
{
struct Camera;
struct Grid;

class RenderSystem : public ISystem
{
public:
	RenderSystem( entt::registry& registry, IRenderService& renderer );
	auto update( entt::registry& registry ) -> void override;

private:
	auto updateGridHandle( const Grid& grid ) -> void;
	auto updateCameraHandle( const Camera& camera ) -> void;

	IRenderService& m_renderer;
	GridHandle m_gridHandle;
	CameraHandle m_cameraHandle;
};
}  // namespace cc::app