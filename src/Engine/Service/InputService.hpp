#pragma once

#include <entt/signal/fwd.hpp>
#include <glm/vec2.hpp>
#include <magic_enum/magic_enum.hpp>

#include "Engine/Events/WindowEvents.hpp"
#include "Engine/Interface/IService.hpp"

namespace cc
{
class InputService : public IEndFrameService
{
public:
	auto init( entt::registry& registry ) -> void override;

	auto endFrame( entt::registry& registry ) -> void override;

	[[nodiscard]] auto isPressed( keyboard::Key key ) const -> bool;
	[[nodiscard]] auto isPressed( mouse::Button button ) const -> bool;

	[[nodiscard]] auto isReleased( keyboard::Key key ) const -> bool;
	[[nodiscard]] auto isReleased( mouse::Button button ) const -> bool;

	[[nodiscard]] auto isUp( keyboard::Key key ) const -> bool;
	[[nodiscard]] auto isUp( mouse::Button button ) const -> bool;

	[[nodiscard]] auto isDown( keyboard::Key key ) const -> bool;
	[[nodiscard]] auto isDown( mouse::Button button ) const -> bool;

	[[nodiscard]] auto getLastMousePos() const -> glm::vec2;

private:
	struct Keyboard
	{
		static constexpr int keyCount = magic_enum::enum_count< keyboard::Key >();
		std::array< bool, keyCount > current{};
		std::array< bool, keyCount > previous{};
	};

	struct Mouse
	{
		static constexpr int buttonCount = magic_enum::enum_count< mouse::Button >();
		std::array< bool, buttonCount > current{};
		std::array< bool, buttonCount > previous{};
		glm::vec2 lastPosition{ 0.f };
	};

	auto onButtonChanged( const event::MouseButtonChanged& event ) -> void;
	auto onKeyChanged( const event::KeyChanged& event ) -> void;
	auto onFocusLost() -> void;

	Keyboard m_keyboard;
	Mouse m_mouse;
};
}  // namespace cc