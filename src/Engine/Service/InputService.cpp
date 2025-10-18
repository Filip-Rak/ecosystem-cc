#include "InputService.hpp"

#include <entt/entt.hpp>

#include "Engine/Events/WindowEvents.hpp"

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

InputService::InputService( entt::dispatcher& dispatcher )
{
	// clang-format off
	dispatcher.sink< event::MouseButtonChanged >().connect< &InputService::onButtonChanged >(*this );
	dispatcher.sink< event::KeyChanged >().connect< &InputService::onKeyChanged >( *this );
	dispatcher.sink< event::LostFocus >().connect< &InputService::onFocusLost >( *this );
	// clang-format on
}

auto InputService::endFrame( entt::registry& /*registry*/ ) -> void
{
	m_keyboard.previous = m_keyboard.current;
	m_mouse.previous = m_mouse.current;
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

auto InputService::getLastMousePos() const -> glm::vec2
{
	return m_mouse.lastPosition;
}

auto InputService::onButtonChanged( const event::MouseButtonChanged& event ) -> void
{
	const auto buttonIndex = toButtonIndex( event.button );
	m_mouse.current[ buttonIndex ] = event.pressed;
	m_mouse.lastPosition = event.position;
}

auto InputService::onKeyChanged( const event::KeyChanged& event ) -> void
{
	const auto keyIndex = toKeyIndex( event.key );
	m_keyboard.current[ keyIndex ] = event.pressed;
}

auto InputService::onFocusLost() -> void
{
	m_keyboard.current.fill( false );
	m_keyboard.previous.fill( false );

	m_mouse.current.fill( false );
	m_mouse.previous.fill( false );
}
}  // namespace cc