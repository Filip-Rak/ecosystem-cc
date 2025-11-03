#include "InputService.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Engine/ContextEntity/InputMap.hpp"

namespace cc
{
namespace
{
// TODO: Remove if unused.
auto initKeyboardVector( entt::registry& registry )
    -> std::array< bool, magic_enum::enum_count< keyboard::Key >() >&
{
	auto& inputMap = registry.ctx().get< InputMap >();
	return inputMap.keySates;
}
// TODO: Remove if unused.
auto initMouseVector( entt::registry& registry )
    -> std::array< bool, magic_enum::enum_count< mouse::Button >() >&
{
	auto& inputMap = registry.ctx().get< InputMap >();
	return inputMap.buttonSates;
}

auto toKeyIndex( keyboard::Key key ) -> uint8_t
{
	return static_cast< uint8_t >( key );
}

auto toButtonIndex( mouse::Button button ) -> uint8_t
{
	return static_cast< uint8_t >( button );
}
}  // namespace

InputService::InputService( entt::registry& registry )
{
	assert( registry.ctx().contains< InputMap >() && "InputMap not initialized" );
}

auto InputService::beginFrame( entt::registry& registry ) -> void
{
	// TODO: Copy or ref?
	if ( const auto& inputMap = registry.ctx().get< InputMap >(); !inputMap.windowInFocus )
	{
		zeroInput();
	}
	else
	{
		m_keyboard.current = inputMap.keySates;
		m_mouse.current = inputMap.buttonSates;
		m_mouse.currentPosition = inputMap.mousePos;
	}
}

auto InputService::endFrame( entt::registry& /*registry*/ ) -> void
{
	m_keyboard.previous = m_keyboard.current;
	m_mouse.previous = m_mouse.current;
	m_mouse.lastPosition = m_mouse.currentPosition;
}

auto InputService::isPressed( keyboard::Key key ) const -> bool
{
	const auto keyIndex = toKeyIndex( key );
	return m_keyboard.current[ keyIndex ] && !m_keyboard.previous[ keyIndex ];
}

auto InputService::isPressed( mouse::Button button ) const -> bool
{
	const auto buttonIndex = toButtonIndex( button );
	return m_mouse.current[ buttonIndex ] && !m_mouse.previous[ buttonIndex ];
}

auto InputService::isReleased( keyboard::Key key ) const -> bool
{
	const auto keyIndex = toKeyIndex( key );
	return !m_keyboard.current[ keyIndex ] && m_keyboard.previous[ keyIndex ];
}

auto InputService::isReleased( mouse::Button button ) const -> bool
{
	const auto buttonIndex = toButtonIndex( button );
	return !m_mouse.current[ buttonIndex ] && m_mouse.previous[ buttonIndex ];
}

auto InputService::isUp( keyboard::Key key ) const -> bool
{
	const auto keyIndex = toKeyIndex( key );
	return !m_keyboard.current[ keyIndex ];
}

auto InputService::isUp( mouse::Button button ) const -> bool
{
	const auto buttonIndex = toButtonIndex( button );
	return !m_mouse.current[ buttonIndex ];
}

auto InputService::isDown( keyboard::Key key ) const -> bool
{
	const auto keyIndex = toKeyIndex( key );
	return m_keyboard.current[ keyIndex ];
}

auto InputService::isDown( mouse::Button button ) const -> bool
{
	const auto buttonIndex = toButtonIndex( button );
	return m_mouse.current[ buttonIndex ];
}

auto InputService::getCurrentMousePos() const -> glm::ivec2
{
	return m_mouse.currentPosition;
}

auto InputService::getLastMousePos() const -> glm::ivec2
{
	return m_mouse.lastPosition;
}

auto InputService::getMouseMoveDelta() const -> glm::ivec2
{
	return m_mouse.lastPosition - m_mouse.currentPosition;
}

auto InputService::zeroInput() -> void
{
	m_keyboard.current.fill( false );
	m_keyboard.previous.fill( false );

	m_mouse.current.fill( false );
	m_mouse.previous.fill( false );
}
}  // namespace cc