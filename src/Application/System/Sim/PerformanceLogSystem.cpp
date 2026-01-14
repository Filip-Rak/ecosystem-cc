#include "Application/System/Sim/PerformanceLogSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>
#include <memory>

#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Logger.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/Utility/PerformanceLog.hpp"
#include "Engine/ContextEntity/Time.hpp"

namespace cc::app
{
PerformanceLogSystem::PerformanceLogSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< std::unique_ptr< Logger > >() );
	assert( registry.ctx().contains< SimRunnerData >() );
	assert( registry.ctx().contains< Time >() );
	assert( registry.ctx().contains< Grid >() );
}

auto PerformanceLogSystem::update() -> void
{
	// Log data of the previous frame with current (previous) time.
	if ( !m_firstFrame )
	{
		const auto& logger     = m_registry.ctx().get< std::unique_ptr< Logger > >();
		const auto& runnerData = m_registry.ctx().get< SimRunnerData >();
		const auto& time       = m_registry.ctx().get< Time >();

		const PerformanceLog log{ .iteration  = runnerData.iteration - 1,
		                          .agentCount = m_previousAgentCount,
		                          .frameTime  = time.deltaTime,
		                          .tickTime   = runnerData.tickTime };

		logger->writeTickPerformance( log );
	}

	// Buffer this frame data.
	const auto& grid     = m_registry.ctx().get< Grid >();
	m_previousAgentCount = grid.getPopulation();

	m_firstFrame = false;
}
}  // namespace cc::app