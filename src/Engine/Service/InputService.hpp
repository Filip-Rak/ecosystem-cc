#pragma once

#include <entt/signal/fwd.hpp>
#include <glm/vec2.hpp>
#include <magic_enum/magic_enum.hpp>

#include "Engine/Interface/IService.hpp"
#include "Engine/Utility/InputEnums.hpp"

namespace cc
{
namespace event
{
struct LostFocus;
struct GainedFocus;
}  // namespace event

class InputService : public IFrameService
{
public:
	InputService( entt::registry& registry );

	auto beginFrame( entt::registry& registry ) -> void override;
	auto endFrame( entt::registry& registry ) -> void override;

	[[nodiscard]] auto isPressed( keyboard::Key key ) const -> bool;
	[[nodiscard]] auto isPressed( mouse::Button button ) const -> bool;

	[[nodiscard]] auto isReleased( keyboard::Key key ) const -> bool;
	[[nodiscard]] auto isReleased( mouse::Button button ) const -> bool;

	[[nodiscard]] auto isUp( keyboard::Key key ) const -> bool;
	[[nodiscard]] auto isUp( mouse::Button button ) const -> bool;

	[[nodiscard]] auto isDown( keyboard::Key key ) const -> bool;
	[[nodiscard]] auto isDown( mouse::Button button ) const -> bool;

	[[nodiscard]] auto getCurrentMousePos() const -> glm::ivec2;
	[[nodiscard]] auto getLastMousePos() const -> glm::ivec2;
	[[nodiscard]] auto getMouseMoveDelta() const -> glm::ivec2;
	[[nodiscard]] auto getMouseScrollDelta() const -> float;

private:
	struct Keyboard
	{
		static constexpr int KeyCount = magic_enum::enum_count< keyboard::Key >();
		std::array< bool, KeyCount > current{};
		std::array< bool, KeyCount > previous{};
	};

	struct Mouse
	{
		static constexpr int ButtonCount = magic_enum::enum_count< mouse::Button >();
		std::array< bool, ButtonCount > current{};
		std::array< bool, ButtonCount > previous{};
		glm::ivec2 currentPosition{ 0 };
		glm::ivec2 lastPosition{ 0 };
		float scrollDelta = 0.f;
	};

	auto zeroInput() -> void;
	auto onLoseFocus( const event::LostFocus& lostFocus ) -> void;

	Keyboard m_keyboard;
	Mouse m_mouse;
	bool m_wasIsInFocus = false;
};
}  // namespace cc