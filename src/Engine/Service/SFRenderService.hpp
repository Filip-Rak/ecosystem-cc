#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Engine/Interface/IRenderService.hpp"

namespace cc
{
struct Color;

class SFRenderService : public IRenderService
{
public:
	SFRenderService( sf::RenderWindow& window );

	auto beginFrame( entt::registry& registry ) -> void override;
	auto endFrame( entt::registry& registry ) -> void override;

private:
	sf::RenderWindow& m_window;
};
}  // namespace cc