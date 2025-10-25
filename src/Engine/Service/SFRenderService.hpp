#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Engine/Interface/IRenderService.hpp"

namespace cc
{
namespace event
{
struct RebuildFont;
}

struct Color;

class SFRenderService : public IRenderService
{
public:
	SFRenderService( entt::registry& registry, sf::RenderWindow& window );

	auto beginFrame( entt::registry& registry ) -> void override;
	auto endFrame( entt::registry& registry ) -> void override;

private:
	auto onRebuildFont( const event::RebuildFont& event ) -> void;
	bool m_updatedFont = true;
	sf::RenderWindow& m_window;
};
}  // namespace cc