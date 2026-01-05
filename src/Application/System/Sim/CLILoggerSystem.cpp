#include "Application/System/Sim/CLILoggerSystem.hpp"

#include <cassert>
#include <print>

#include <entt/entt.hpp>

#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/Events/SimRunnerEvents.hpp"

namespace cc::app
{
namespace
{
auto onExtinction( const event::Extinction& event ) -> void
{
	std::println( "Stopped. Agents went extinct in iteration: {}", event.iteration );
}
}  // namespace

CLILoggerSystem::CLILoggerSystem( entt::registry& registry, int logfrequency )
    : m_registry( registry ), m_logfrequency( logfrequency )
{
	assert( registry.ctx().contains< entt::dispatcher >() );
	assert( registry.ctx().contains< SimRunnerData >() );
	assert( registry.ctx().contains< Preset >() );

	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::Extinction >().connect< &onExtinction >();
}

auto CLILoggerSystem::update() -> void
{
	const auto& runnerData = m_registry.ctx().get< SimRunnerData >();
	const auto& preset     = m_registry.ctx().get< Preset >();

	const auto iteration = runnerData.iteration;
	const auto target    = preset.iterationTarget;

	if ( iteration % m_logfrequency != 0 && iteration < target ) return;

	constexpr int barWidth = 50;
	const int progress     = static_cast< int >( barWidth * iteration / target );

	std::string bar;
	bar.reserve( barWidth );  // FIXME: reserve every frame?

	for ( int i = 0; i < barWidth; i++ )
	{
		if ( i < progress )
			bar += '=';
		else if ( i == progress )
			bar += '>';
		else
			bar += ' ';
	}

	std::string line = std::format( "\r[{}] iteration {} of {}", bar, iteration, target );
	if ( iteration == target ) line += "\nFinished\n";
	std::print( "{}", line );
}
}  // namespace cc::app