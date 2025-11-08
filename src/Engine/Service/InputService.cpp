#include "InputService.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Engine/ContextEntity/InputMap.hpp"

namespace cc
{
namespace
{
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
	assert( registry.ctx().contains< entt::dispatcher >() && "Dispatcher not initialized" );
	assert( registry.ctx().contains< InputMap >() && "InputMap not initialized" );

	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::LostFocus >().connect< &InputService::onLoseFocus >( *this );
}

auto InputService::beginFrame( entt::registry& registry ) -> void
{
	// TODO: Copy or ref?
	const auto& inputMap = registry.ctx().get< InputMap >();
	if ( inputMap.windowInFocus )
	{
		m_keyboard.current = inputMap.keySates;
		m_mouse.current = inputMap.buttonSates;
		m_mouse.currentPosition = inputMap.mousePos;
		m_mouse.scrollDelta = inputMap.mouseScrollDelta;

		if ( !m_wasIsInFocus )
		{
			m_mouse.lastPosition = m_mouse.currentPosition;
			m_wasIsInFocus = true;
		}
	}

	m_wasIsInFocus = inputMap.windowInFocus;
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

auto InputService::getMouseScrollDelta() const -> float
{
	return m_mouse.scrollDelta;
}

auto InputService::zeroInput() -> void
{
	// TODO: Rebuild structs instead?
	m_keyboard.current.fill( false );
	m_keyboard.previous.fill( false );

	m_mouse.current.fill( false );
	m_mouse.previous.fill( false );
	m_mouse.scrollDelta = 0.f;
}

auto InputService::onLoseFocus( const event::LostFocus& /*lostFocus*/ ) -> void
{
	zeroInput();
}
}  // namespace cc