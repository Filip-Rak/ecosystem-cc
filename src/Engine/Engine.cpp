#include "Engine/Engine.hpp"

#include <cassert>

#include "Engine/ContextEntity/InputMap.hpp"
#include "Engine/Events/SystemEvents.hpp"
#include "Engine/Events/WindowEvents.hpp"
#include "Engine/Service/GUIService.hpp"
#include "Engine/Service/InputService.hpp"
#include "Engine/Service/SFRenderService.hpp"
#include "Engine/Service/SFWindowService.hpp"
#include "Engine/Service/TimeService.hpp"

namespace cc
{
Engine::Engine( const Args& args )
{
	initWindowEntities();
	setupDispatcher();
	createCoreServices();

	if ( args.EnableGUI )
	{
		initWindowEntities();
		createWindowServices( args );
	}
};

auto Engine::run() -> void
{
	auto& dispatcher = m_registry.ctx().get< entt::dispatcher >();
	while ( m_isRunning )
	{
		for ( auto& service : m_services ) service->beginFrame( m_registry );
		for ( auto& system : m_systems ) system->update();
		for ( auto& service : m_services ) service->endFrame( m_registry );

		dispatcher.update();
	}
}

auto Engine::registry() -> entt::registry&
{
	return m_registry;
}

auto Engine::initWindowEntities() -> void
{
	m_registry.ctx().emplace< InputMap >();
}

auto Engine::setupDispatcher() -> void
{
	auto& dispatcher = m_registry.ctx().emplace< entt::dispatcher >();
	dispatcher.sink< event::Exit >().connect< &Engine::onExit >( *this );
	dispatcher.sink< event::WindowClosed >().connect< &Engine::onWindowClosed >( *this );
}

auto Engine::createCoreServices() -> void
{
	addService< TimeService >( m_registry );
}

auto Engine::createWindowServices( const Args& args ) -> void
{
	auto& window = addService< SFWindowService >( m_registry, args.WindowWidth, args.WindowHeight, args.Title );
	addService< InputService >( m_registry );
	addService< GUIService >();
	addService< SFRenderService >( m_registry, window.getWindow() );
}

auto Engine::onExit( const event::Exit& /*exitEvent*/ ) -> void
{
	m_isRunning = false;
}

auto Engine::onWindowClosed( const event::WindowClosed& /*event*/ ) -> void
{
	m_isRunning = false;
}
}  // namespace cc