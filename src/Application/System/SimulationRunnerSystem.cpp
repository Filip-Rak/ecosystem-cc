#include "Application/System/SimulationRunnerSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/System/VegetationSystem.hpp"
#include "Engine/ContextEntity/Time.hpp"

namespace cc::app
{
SimulationRunnerSystem::SimulationRunnerSystem( entt::registry& registry )
{
	assert( registry.ctx().contains< SimRunnerData >() && "SimRunnerData not initialized" );
	assert( registry.ctx().contains< Time >() && "Time not initialized" );

	m_nestedSystems.push_back( std::make_unique< VegetationSystem >( registry ) );
}

auto SimulationRunnerSystem::update( entt::registry& registry ) -> void
{
	auto& data = registry.ctx().get< SimRunnerData >();
	const float timeBetweenUpdates = 1.f / static_cast< float >( data.speed );
	if ( data.paused )
	{
		m_timeSinceLastUpdate = timeBetweenUpdates;
		return;
	}

	bool timeToUpdate = m_timeSinceLastUpdate > timeBetweenUpdates;
	if ( !timeToUpdate )
	{
		const auto& time = registry.ctx().get< Time >();
		m_timeSinceLastUpdate += time.DeltaTime;
		return;
	}

	m_timeSinceLastUpdate = 0.f;
	for ( auto& system : m_nestedSystems )
	{
		system->update( registry );
	}

	data.iteration = ++m_iteration;
}
}  // namespace cc::app