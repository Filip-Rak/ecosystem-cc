#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Engine/Events/SystemEvents.hpp"
#include "Engine/Interface/IService.hpp"

namespace cc::keyboard
{
enum class Key : uint8_t;
}

namespace cc
{
struct InputMap;
class SFWindowService : public IBeginFrameService
{
public:
	SFWindowService( entt::registry& registry, uint16_t width, uint16_t height,
	                 const std::string& title );

	~SFWindowService();

	auto beginFrame( entt::registry& registry ) -> void override;

	[[nodiscard]] auto isOpen() const -> bool;
	[[nodiscard]] auto inFocus() const -> bool;
	[[nodiscard]] auto getWindow() -> sf::RenderWindow&;

private:
	auto onExit( const event::Exit& exitEvent ) -> void;
	auto publishWindowEvents( entt::dispatcher& dispatcher, InputMap& inputMap ) -> void;
	[[nodiscard]] auto pollEvents() -> std::vector< sf::Event >;

	sf::RenderWindow m_window;
	sf::Clock m_imguiClock;
	bool m_inFocus{ true };
};
}  // namespace cc