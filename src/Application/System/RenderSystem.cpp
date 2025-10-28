#include "Application/System/RenderSystem.hpp"

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

#include "Engine/Service/InputService.hpp"  // TODO : Debug
#include "Engine/Utility/Time.hpp"          // TODO : Debug

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
{}

auto RenderSystem::update( entt::registry& registry ) -> void
{
	// TODO: Move to InputSystem.
	auto& input = registry.ctx().get< InputService >();
	auto& time = registry.ctx().get< Time >();

	glm::vec2 movementInput{ 0.f };
	if ( input.isDown( keyboard::Key::W ) ) movementInput.y -= 1;
	if ( input.isDown( keyboard::Key::S ) ) movementInput.y += 1;
	if ( input.isDown( keyboard::Key::A ) ) movementInput.x -= 1;
	if ( input.isDown( keyboard::Key::D ) ) movementInput.x += 1;

	constexpr float movementSpeed = 100.f;
	const glm::vec2 movementOffset = movementInput * movementSpeed * time.DeltaTime;
	m_renderer.moveCamera( m_camera, movementOffset );

	float zoomInput = 0.f;
	if ( input.isDown( keyboard::Key::Up ) ) zoomInput -= 1.f;
	if ( input.isDown( keyboard::Key::Down ) ) zoomInput += 1.f;

	constexpr float zoomSpeed = 0.1f;
	const float zoomDelta = zoomInput * zoomSpeed * time.DeltaTime;
	m_renderer.zoomCamera( m_camera, zoomDelta );

	m_renderer.setCamera( m_camera );
	m_renderer.draw( m_grid );
}
}  // namespace cc::app