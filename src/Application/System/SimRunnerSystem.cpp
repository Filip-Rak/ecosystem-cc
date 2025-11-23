#include "Application/System/SimRunnerSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/Events/SimRunnerEvents.hpp"
#include "Application/System/VegetationSystem.hpp"
#include "Engine/ContextEntity/Time.hpp"

namespace cc::app
{
SimRunnerSystem::SimRunnerSystem( entt::registry& registry, bool speedLimited )
    : m_initialGrid( registry.ctx().get< Grid >() ), m_speedLimited( speedLimited ), m_registry( registry )
{
	assert( registry.ctx().contains< entt::dispatcher >() && "Dispatcher not initialized" );
	assert( registry.ctx().contains< SimRunnerData >() && "SimRunnerData not initialized" );
	assert( registry.ctx().contains< Time >() && "Time not initialized" );

	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::ResetGrid >().connect< &SimRunnerSystem::onResetGrid >( *this );

	m_nestedSystems.push_back( std::make_unique< VegetationSystem >( registry ) );
}

auto SimRunnerSystem::update() -> void
{
	auto& data = m_registry.ctx().get< SimRunnerData >();
	auto& preset = m_registry.ctx().get< Preset >();

	if ( m_speedLimited && !shouldUpdate( data, m_registry.ctx().get< Time >() ) )
	{
		return;
	}

	for ( auto& system : m_nestedSystems )
	{
		system->update();
	}

	m_timeSinceLastUpdate = 0.f;
	data.iteration = ++m_iteration;

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