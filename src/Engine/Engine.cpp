#include "Engine/Engine.hpp"

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

	auto& win = m_registry.ctx().emplace< SFWindowService >( dispatcher, args.WindowWidth,
	                                                         args.WindowHeight, args.Title );
	m_services.emplace_back( win );

	auto& renderer = m_registry.ctx().emplace< SFRenderService >( win.getWindow() );
	m_services.emplace_back( renderer );

	auto& timeService = m_registry.ctx().emplace< TimeService >();
	m_services.emplace_back( timeService );

	auto& inputService = m_registry.ctx().emplace< InputService >( dispatcher );
	m_services.emplace_back( inputService );
};

auto Engine::run() -> void
{
	while ( m_registry.ctx().get< SFWindowService >().isOpen() )
	{
		for ( auto& service : m_services ) service.get().beginFrame( m_registry );
		for ( auto& system : m_systems ) system->update( m_registry );
		for ( auto& service : m_services ) service.get().endFrame( m_registry );
	}
}
}  // namespace cc