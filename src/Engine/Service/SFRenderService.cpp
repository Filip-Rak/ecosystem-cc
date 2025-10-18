#include "Engine/Service/SFRenderService.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

namespace cc
{
SFRenderService::SFRenderService( sf::RenderWindow& window ) : m_window( window ) {}

auto SFRenderService::beginFrame( entt::registry& /*registry*/ ) -> void
{
	m_window.clear();
}

auto SFRenderService::endFrame( entt::registry& /*registry*/ ) -> void
{
	m_window.display();
}
}  // namespace cc