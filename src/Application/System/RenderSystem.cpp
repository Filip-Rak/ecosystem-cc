#include "Application/System/RenderSystem.hpp"

#include "Engine/Service/InputService.hpp"  // TODO : Debug

#include <entt/entt.hpp>

#include <glm/vec2.hpp>

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
    : m_renderer( renderer ), m_grid( initGrid( registry, renderer ) )
{}

auto RenderSystem::update( entt::registry& registry ) -> void
{
	auto& input = registry.ctx().get< InputService >();

	if ( input.isDown( keyboard::Key::W ) ) m_renderer.draw( m_grid );
}
}  // namespace cc::app