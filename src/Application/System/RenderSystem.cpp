#include "Application/System/RenderSystem.hpp"

#include <cassert>
#include <cstddef>
#include <vector>

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Engine/Utility/Color.hpp"

namespace cc::app
{
namespace
{
auto initGridHandle( entt::registry& /*registry*/, IRenderService& renderer ) -> GridHandle
{
	// TODO: Base visual grid on data from registry.
	// WARN: Same as App.
	constexpr uint16_t FixedDim = 100;
	return renderer.createGrid( FixedDim, FixedDim, glm::vec2{ 0.f }, 4.f );
}
}  // namespace
RenderSystem::RenderSystem( entt::registry& registry, IRenderService& renderer )
    : m_renderer( renderer ),
      m_gridHandle( initGridHandle( registry, renderer ) ),
      m_cameraHandle( renderer.createCamera() )
{
	assert( registry.ctx().contains< Camera >() && "Camera not initialized" );
}

auto RenderSystem::update( entt::registry& registry ) -> void
{
	const auto& grid = registry.ctx().get< Grid >();
	updateGridHandle( grid );

	const auto& camera = registry.ctx().get< Camera >();
	updateCameraHandle( camera );
}

auto RenderSystem::updateGridHandle( const Grid& grid ) -> void
{
	// TODO: Use actual smart coloring.
	std::vector< Color > colors( grid.cellsRowMajor.size(),
	                             Color{ .red = 126, .green = 255, .blue = 240 } );

	m_renderer.setGridColors( m_gridHandle, colors );
}

auto RenderSystem::updateCameraHandle( const Camera& camera ) -> void
{
	m_renderer.setPosition( m_cameraHandle, camera.position );
	m_renderer.setZoom( m_cameraHandle, camera.zoomLevel );

	m_renderer.setActiveCamera( m_cameraHandle );
	m_renderer.draw( m_gridHandle );
}
}  // namespace cc::app