#include "Application/System/RenderSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

#include "Application/ContextEntity/Camera.hpp"
namespace cc::app
{
namespace
{
auto initGrid( entt::registry& /*registry*/, IRenderService& renderer ) -> GridHandle
{
	// TODO: Base visual grid on data from registry.
	// WARN: Same as App.
	constexpr uint16_t FixedDim = 100;
	return renderer.createGrid( FixedDim, FixedDim, glm::vec2{ 0.f }, 4.f );
}
}  // namespace
RenderSystem::RenderSystem( entt::registry& registry, IRenderService& renderer )
    : m_renderer( renderer ),
      m_grid( initGrid( registry, renderer ) ),
      m_camera( renderer.createCamera() )
{
	assert( registry.ctx().contains< Camera >() && "Camera not initialized" );
}

auto RenderSystem::update( entt::registry& registry ) -> void
{
	auto& camera = registry.ctx().get< Camera >();

	// TODO: if?
	m_renderer.setPosition( m_camera, camera.position );
	m_renderer.setZoom( m_camera, camera.zoomLevel );

	m_renderer.setCamera( m_camera );
	m_renderer.draw( m_grid );
}
}  // namespace cc::app