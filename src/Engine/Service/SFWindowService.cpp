#include "Engine/Service/SFWindowService.hpp"

#include <SFML/Window/Event.hpp>
#include <entt/entt.hpp>
#include <magic_enum/magic_enum.hpp>

#include "Engine/Events/WindowEvents.hpp"

namespace
{
using namespace cc;
[[nodiscard]] auto toCcKey( const sf::Keyboard::Key sfmlKey ) -> keyboard::Key
{
	constexpr uint8_t Offset = 1;
	return static_cast< keyboard::Key >( static_cast< int8_t >( sfmlKey ) + Offset );
}

[[nodiscard]] auto toCcButton( const sf::Mouse::Button button ) -> mouse::Button
{
	if ( magic_enum::enum_index< sf::Mouse::Button >( button ) <=
	     magic_enum::enum_index< sf::Mouse::Button >( sf::Mouse::Button::Middle ) )
		return static_cast< mouse::Button >( button );

	return mouse::Button::Unknown;
}

template < typename T >
[[nodiscard]] auto toVec( const sf::Vector2< T > sfVector )
{
	return glm::vec< 2, T >( sfVector.x, sfVector.y );
}
}  // namespace

namespace cc
{
SFWindowService::SFWindowService( entt::dispatcher& dispatcher, uint16_t width, uint16_t height,
                                  const std::string& title )
    : m_window( sf::VideoMode( { width, height } ), title )
{
	dispatcher.sink< event::Exit >().connect< &SFWindowService::onExit >( *this );
}

auto SFWindowService::beginFrame( entt::registry& registry ) -> void
{
	auto& dispatcher = registry.ctx().get< entt::dispatcher >();

	publishWindowEvents( dispatcher );
	dispatcher.update();
}

auto SFWindowService::isOpen() const -> bool
{
	return m_window.isOpen();
}

auto SFWindowService::inFocus() const -> bool
{
	return m_inFocus;
}

auto SFWindowService::getWindow() -> sf::RenderWindow&
{
	return m_window;
}

auto SFWindowService::onExit( const event::Exit& /*exitEvent*/ ) -> void
{
	m_window.close();
}

auto SFWindowService::pollEvents() -> std::vector< sf::Event >
{
	std::vector< sf::Event > events;
	while ( const auto event = m_window.pollEvent() )
	{
		if ( const auto* closed = event->getIf< sf::Event::Closed >() )
		{
			m_window.close();
		}
		else if ( const auto* focusGained = event->getIf< sf::Event::FocusGained >() )
		{
			m_inFocus = true;
		}
		else if ( const auto* focusLost = event->getIf< sf::Event::FocusLost >() )
		{
			m_inFocus = false;
		}
		else if ( const auto* windowResized = event->getIf< sf::Event::Resized >() )
		{
			const auto size = windowResized->size;
			const auto newX = static_cast< float >( size.x );
			const auto newY = static_cast< float >( size.y );
			const sf::FloatRect area( { 0.f, 0.f }, { newX, newY } );

			m_window.setView( sf::View( area ) );
		}

		events.emplace_back( *event );
	}

	return events;
}

auto SFWindowService::publishWindowEvents( entt::dispatcher& dispatcher ) -> void
{
	const auto events = pollEvents();
	for ( auto event : events )
	{
		if ( const auto* closed = event.getIf< sf::Event::Closed >() )
		{
			dispatcher.enqueue< event::WindowClosed >();
		}
		else if ( const auto* lostFocus = event.getIf< sf::Event::FocusLost >() )
		{
			dispatcher.enqueue< event::LostFocus >();
		}
		else if ( const auto* gainedFocus = event.getIf< sf::Event::FocusGained >() )
		{
			dispatcher.enqueue< event::GainedFocus >();
		}
		else if ( const auto* windowResized = event.getIf< sf::Event::Resized >() )
		{
			const auto width = static_cast< uint16_t >( windowResized->size.x );
			const auto height = static_cast< uint16_t >( windowResized->size.y );
			dispatcher.enqueue< event::WindowResized >( width, height );
		}
		else if ( const auto* keyPressed = event.getIf< sf::Event::KeyPressed >() )
		{
			if ( !m_inFocus ) return;

			const auto key = keyPressed->code;
			dispatcher.enqueue< event::KeyChanged >( toCcKey( key ), true );
		}
		else if ( const auto* keyReleased = event.getIf< sf::Event::KeyReleased >() )
		{
			if ( !m_inFocus ) return;

			const auto key = keyReleased->code;
			dispatcher.enqueue< event::KeyChanged >( toCcKey( key ), false );
		}
		else if ( const auto* mousePressed = event.getIf< sf::Event::MouseButtonPressed >() )
		{
			if ( !m_inFocus ) return;

			const auto button = mousePressed->button;
			const auto position = mousePressed->position;
			dispatcher.enqueue< event::MouseButtonChanged >( toCcButton( button ),
			                                                 toVec( position ), true );
		}
		else if ( const auto* mouseReleased = event.getIf< sf::Event::MouseButtonReleased >() )
		{
			if ( !m_inFocus ) return;

			const auto button = mouseReleased->button;
			const auto position = mouseReleased->position;
			dispatcher.enqueue< event::MouseButtonChanged >( toCcButton( button ),
			                                                 toVec( position ), false );
		}
	}
}
}  // namespace cc