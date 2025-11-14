#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <entt/fwd.hpp>

#include "Engine/Events/SystemEvents.hpp"
#include "Engine/Interface/IService.hpp"

namespace cc::keyboard
{
enum class Key : uint8_t;
}

namespace cc
{
struct InputMap;
class SFWindowService : public IFrameService
{
public:
	SFWindowService( entt::registry& registry, uint16_t width, uint16_t height, const std::string& title );

	~SFWindowService();

	auto beginFrame() -> void override;
	auto endFrame() -> void override;

	[[nodiscard]] auto isOpen() const -> bool;
	[[nodiscard]] auto inFocus() const -> bool;
	[[nodiscard]] auto getWindow() -> sf::RenderWindow&;

private:
	auto handleWindowEvents( entt::dispatcher& dispatcher, InputMap& inputMap ) -> void;
	[[nodiscard]] auto pollEvents() -> std::vector< sf::Event >;
	auto adaptEvent( const sf::Event& event, entt::dispatcher& dispatcher, InputMap& inputMap ) const -> void;

	auto onExit( const event::Exit& exitEvent ) -> void;

	entt::registry& m_registry;
	sf::RenderWindow m_window;
	sf::Clock m_imguiClock;
	bool m_inFocus{ true };
};
}  // namespace cc