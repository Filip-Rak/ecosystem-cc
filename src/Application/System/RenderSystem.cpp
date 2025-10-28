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

	glm::vec2 inputValue{ 0.f };
	if ( input.isDown( keyboard::Key::W ) ) inputValue.y -= 1;
	if ( input.isDown( keyboard::Key::S ) ) inputValue.y += 1;
	if ( input.isDown( keyboard::Key::A ) ) inputValue.x -= 1;
	if ( input.isDown( keyboard::Key::D ) ) inputValue.x += 1;

	constexpr float Speed = 100.f;
	const glm::vec2 offset = inputValue * Speed * time.DeltaTime;

	m_renderer.moveCamera( m_camera, offset );
	m_renderer.setCamera( m_camera );

	m_renderer.draw( m_grid );
}
}  // namespace cc::app