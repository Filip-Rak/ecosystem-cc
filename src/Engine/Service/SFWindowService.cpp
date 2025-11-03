#include "Engine/Service/SFWindowService.hpp"

#include <cstdint>

#include <SFML/Window/Event.hpp>
#include <entt/entt.hpp>
#include <imgui-SFML.h>
#include <magic_enum/magic_enum.hpp>

#include "Engine/ContextEntity/InputMap.hpp"
#include "Engine/Events/WindowEvents.hpp"
#include "Engine/Utility/SFMath.hpp"

namespace cc
{
namespace
{
constexpr uint8_t KeyboardOffset = 1;

[[nodiscard]] auto toCcKey( const sf::Keyboard::Key sfmlKey ) -> keyboard::Key
{
	const auto ccIndex = static_cast< int8_t >( sfmlKey ) + KeyboardOffset;
	return static_cast< keyboard::Key >( ccIndex );
}

[[nodiscard]] auto toCcButton( const sf::Mouse::Button button ) -> mouse::Button
{
	if ( magic_enum::enum_index< sf::Mouse::Button >( button ) <=
	     magic_enum::enum_index< sf::Mouse::Button >( sf::Mouse::Button::Middle ) )
		return static_cast< mouse::Button >( button );

	return mouse::Button::Unknown;
}

auto updateInputMap( InputMap& inputMap, keyboard::Key ccKey, bool pressed ) -> void
{
	const auto ccKeyIndex = static_cast< std::size_t >( ccKey );
	inputMap.keySates[ ccKeyIndex ] = pressed;
}

auto updateInputMap( InputMap& inputMap, mouse::Button ccButton, bool pressed ) -> void
{
	const auto buttonIndex = static_cast< std::size_t >( ccButton );
	inputMap.buttonSates[ buttonIndex ] = pressed;
}
auto updateInputMap( InputMap& inputMap, bool inFocus ) -> void
{
	inputMap.windowInFocus = inFocus;
}

auto updateInputMap( InputMap& inputMap, sf::Vector2i mousePos ) -> void
{
	inputMap.mousePos = toGlm( mousePos );
}
}  // namespace

SFWindowService::SFWindowService( entt::registry& registry, uint16_t width, uint16_t height,
                                  const std::string& title )
    : m_window( sf::VideoMode( { width, height } ), title )
{
	assert( registry.ctx().contains< entt::dispatcher >() && "entt::dispatcher not initialized" );
	assert( registry.ctx().contains< InputMap >() && "InputMap not initialized" );
	assert( ImGui::SFML::Init( m_window ) && "Creation of imgui context for SFML Failed" );

	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::Exit >().connect< &SFWindowService::onExit >( *this );
}

SFWindowService::~SFWindowService()
{
	ImGui::SFML::Shutdown();
}

auto SFWindowService::beginFrame( entt::registry& registry ) -> void
{
	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	auto& inputMap = registry.ctx().get< InputMap >();

	publishWindowEvents( dispatcher, inputMap );

	const sf::Vector2i mousePos = sf::Mouse::getPosition( m_window );
	updateInputMap( inputMap, mousePos );

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
		if ( const auto* focusGained = event->getIf< sf::Event::FocusGained >() )
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
		ImGui::SFML::ProcessEvent( m_window, *event );
	}

	ImGui::SFML::Update( m_window, m_imguiClock.restart() );
	return events;
}

auto SFWindowService::publishWindowEvents( entt::dispatcher& dispatcher, InputMap& inputMap )
    -> void
{
	const auto events = pollEvents();
	for ( auto event : events )
	{
		if ( const auto* closed = event.getIf< sf::Event::Closed >() )
		{
			// TODO: Make into one event?
			dispatcher.enqueue< event::WindowClosed >();
			dispatcher.enqueue< event::Exit >();
		}
		else if ( const auto* lostFocus = event.getIf< sf::Event::FocusLost >() )
		{
			updateInputMap( inputMap, false );
			dispatcher.enqueue< event::LostFocus >();
		}
		else if ( const auto* gainedFocus = event.getIf< sf::Event::FocusGained >() )
		{
			updateInputMap( inputMap, true );
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

			constexpr bool pressed = true;
			const keyboard::Key ccKey = toCcKey( keyPressed->code );

			dispatcher.enqueue< event::KeyChanged >( ccKey, pressed );
			updateInputMap( inputMap, ccKey, pressed );
		}
		else if ( const auto* keyReleased = event.getIf< sf::Event::KeyReleased >() )
		{
			if ( !m_inFocus ) return;

			constexpr bool Pressed = false;
			const keyboard::Key ccKey = toCcKey( keyReleased->code );

			dispatcher.enqueue< event::KeyChanged >( ccKey, Pressed );
			updateInputMap( inputMap, ccKey, Pressed );
		}
		else if ( const auto* mousePressed = event.getIf< sf::Event::MouseButtonPressed >() )
		{
			if ( !m_inFocus ) return;

			constexpr bool Pressed = true;
			const auto glmPosition = toGlm( mousePressed->position );
			const auto ccButton = toCcButton( mousePressed->button );

			dispatcher.enqueue< event::MouseButtonChanged >( ccButton, glmPosition, Pressed );
			updateInputMap( inputMap, ccButton, Pressed );
		}
		else if ( const auto* mouseReleased = event.getIf< sf::Event::MouseButtonReleased >() )
		{
			if ( !m_inFocus ) return;

			constexpr bool Pressed = false;
			const auto glmPosition = toGlm( mouseReleased->position );
			const auto ccButton = toCcButton( mouseReleased->button );

			dispatcher.enqueue< event::MouseButtonChanged >( ccButton, glmPosition, Pressed );
			updateInputMap( inputMap, ccButton, Pressed );
		}
	}
}
}  // namespace cc