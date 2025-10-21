#include "Engine/Service/SFRenderService.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <imgui-SFML.h>

namespace cc
{
SFRenderService::SFRenderService( sf::RenderWindow& window ) : m_window( window ) {}

auto SFRenderService::beginFrame( entt::registry& /*registry*/ ) -> void
{
	m_window.clear();
}

auto SFRenderService::endFrame( entt::registry& /*registry*/ ) -> void
{
	ImGui::SFML::Render( m_window );
	m_window.display();
}
}  // namespace cc