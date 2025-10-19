#include "Engine/Engine.hpp"

#include "Engine/Events/SystemEvents.hpp"
#include "Engine/Interface/IService.hpp"
#include "Engine/Service/InputService.hpp"
#include "Engine/Service/SFRenderService.hpp"
#include "Engine/Service/SFWindowService.hpp"
#include "Engine/Service/TimeService.hpp"

namespace cc
{
Engine::Engine( const Args& args )
{
	auto& dispatcher = m_registry.ctx().emplace< entt::dispatcher >();
	dispatcher.sink< event::Exit >().connect< &Engine::onExit >( *this );

	auto& timeService = m_registry.ctx().emplace< TimeService >();
	m_services.emplace_back( timeService );

	if ( args.EnableGUI ) initWindowServices( args, dispatcher );
};

auto Engine::run() -> void
{
	while ( m_isRunning )
	{
		for ( auto& service : m_services ) service.get().beginFrame( m_registry );
		for ( auto& system : m_systems ) system->update( m_registry );
		for ( auto& service : m_services ) service.get().endFrame( m_registry );
	}
}

auto Engine::initWindowServices( const Args& args, entt::dispatcher& dispatcher ) -> void
{
	auto& window = m_registry.ctx().emplace< SFWindowService >( dispatcher, args.WindowWidth,
	                                                            args.WindowHeight, args.Title );
	m_services.emplace_back( window );

	auto& renderer = m_registry.ctx().emplace< SFRenderService >( window.getWindow() );
	m_services.emplace_back( renderer );

	auto& inputService = m_registry.ctx().emplace< InputService >( dispatcher );
	m_services.emplace_back( inputService );
}

auto Engine::onExit( const event::Exit& /*exitEvent*/ ) -> void
{
	m_isRunning = false;
}
}  // namespace cc