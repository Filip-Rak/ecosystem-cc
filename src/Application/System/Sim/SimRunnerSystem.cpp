#include "Application/System/Sim/SimRunnerSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/CLI/CLIOptions.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/Events/SimRunnerEvents.hpp"
#include "Application/System/Sim/CLILoggerSystem.hpp"
#include "Application/System/Sim/VegetationSystem.hpp"
#include "Engine/ContextEntity/Time.hpp"

namespace cc::app
{
SimRunnerSystem::SimRunnerSystem( entt::registry& registry, const cc::cli::Options& CLIOptions )
    : m_initialGrid( registry.ctx().get< Grid >() ), m_speedLimited( CLIOptions.gui ), m_registry( registry )
{
	assert( registry.ctx().contains< entt::dispatcher >() && "Dispatcher not initialized" );
	assert( registry.ctx().contains< SimRunnerData >() && "SimRunnerData not initialized" );
	assert( registry.ctx().contains< Preset >() && "Preset not initialized" );
	assert( registry.ctx().contains< Time >() && "Time not initialized" );

	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::ResetGrid >().connect< &SimRunnerSystem::onResetGrid >( *this );

	// Execution -> order matters.
	m_simSystems.push_back( std::make_unique< VegetationSystem >( registry ) );

	if ( !CLIOptions.gui )
	{
		m_simSystems.push_back( std::make_unique< CLILoggerSystem >( registry, CLIOptions.terminalLogInfrequency ) );
	}
}

auto SimRunnerSystem::update() -> void
{
	const auto& preset = m_registry.ctx().get< Preset >();
	auto& data = m_registry.ctx().get< SimRunnerData >();

	if ( m_speedLimited && !shouldUpdate( data, m_registry.ctx().get< Time >() ) )
	{
		return;
	}

	m_timeSinceLastUpdate = 0.f;
	data.iteration = ++m_iteration;

	for ( auto& system : m_simSystems )
	{
		system->update();
	}

	if ( data.iteration >= preset.iterationTarget )
	{
		if ( !data.targetReached )
		{
			data.targetReached = true;
			data.paused = true;

			auto& dispatcher = m_registry.ctx().get< entt::dispatcher >();
			dispatcher.enqueue< event::ReachedTargetIteration >();
		}
	}
}

auto SimRunnerSystem::shouldUpdate( const SimRunnerData& data, const Time& time ) -> bool
{
	const float timeBetweenUpdates = 1.f / static_cast< float >( data.speed );
	if ( data.paused )
	{
		m_timeSinceLastUpdate = timeBetweenUpdates;
		return false;
	}

	const bool timeToUpdate = m_timeSinceLastUpdate > timeBetweenUpdates;
	if ( !timeToUpdate )
	{
		m_timeSinceLastUpdate += time.DeltaTime;
		return false;
	}

	return true;
}

auto SimRunnerSystem::onResetGrid( const event::ResetGrid& /*event*/ ) -> void
{
	m_registry.ctx().erase< Grid >();
	m_registry.ctx().emplace< Grid >( m_initialGrid );

	auto& data = m_registry.ctx().get< SimRunnerData >();
	data.targetReached = false;
	data.iteration = 0;
	m_iteration = 0;
}
}  // namespace cc::app