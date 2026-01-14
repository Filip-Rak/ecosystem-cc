#include "Application/System/Sim/SimRunnerSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/CLI/CLIOptions.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/Randomizer.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/ContextEntity/TickDataCollection.hpp"
#include "Application/Events/SimRunnerEvents.hpp"
#include "Application/System/Sim/AgentAdaptationSystem.hpp"
#include "Application/System/Sim/AgentDecisionSystem.hpp"
#include "Application/System/Sim/AgentFeedingSystem.hpp"
#include "Application/System/Sim/AgentMovementSystem.hpp"
#include "Application/System/Sim/AgentOffspringSystem.hpp"
#include "Application/System/Sim/AgentPassingSystem.hpp"
#include "Application/System/Sim/CLILoggerSystem.hpp"
#include "Application/System/Sim/CellBiomassSystem.hpp"
#include "Application/System/Sim/GuiLogSystem.hpp"
#include "Application/System/Sim/PerformanceLogSystem.hpp"
#include "Application/System/Sim/TickLogSystem.hpp"
#include "Engine/ContextEntity/Time.hpp"
#include "Engine/Utility/TimeUtils.hpp"

namespace cc::app
{
SimRunnerSystem::SimRunnerSystem( entt::registry& registry, const cc::cli::Options& cliOptions )
    : m_limitSpeed( cliOptions.gui && !cliOptions.testPerformance ), m_registry( registry )
{
	assert( registry.ctx().contains< entt::dispatcher >() );
	assert( registry.ctx().contains< SimRunnerData >() );
	assert( registry.ctx().contains< TickDataCollection >() );
	assert( registry.ctx().contains< Preset >() );
	assert( registry.ctx().contains< Time >() );

	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::ResetSim >().connect< &SimRunnerSystem::onResetSim >( *this );

	m_simSystems.emplace_back( std::make_unique< CellBiomassSystem >( registry ) );
	m_simSystems.emplace_back( std::make_unique< AgentDecisionSystem >( registry ) );
	m_simSystems.emplace_back( std::make_unique< AgentMovementSystem >( registry ) );
	m_simSystems.emplace_back( std::make_unique< AgentFeedingSystem >( registry ) );
	m_simSystems.emplace_back( std::make_unique< AgentPassingSystem >( registry ) );
	m_simSystems.emplace_back( std::make_unique< AgentAdaptationSystem >( registry ) );
	m_simSystems.emplace_back( std::make_unique< AgentOffspringSystem >( registry ) );

	const auto& logging = registry.ctx().get< Preset >().logging;
	if ( logging.logPerTickState ) m_simSystems.emplace_back( std::make_unique< TickLogSystem >( registry ) );
	if ( logging.logPerformance ) m_simSystems.emplace_back( std::make_unique< PerformanceLogSystem >( registry ) );

	if ( cliOptions.gui )
		m_simSystems.emplace_back( std::make_unique< GuiLogSystem >( registry ) );
	else
		m_simSystems.emplace_back( std::make_unique< CLILoggerSystem >( registry, cliOptions.terminalLogfrequency ) );
}

auto SimRunnerSystem::update() -> void
{
	if ( m_blockExecution || ( m_limitSpeed && !shouldUpdate() ) )
	{
		return;
	}

	auto& data = m_registry.ctx().get< SimRunnerData >();
	data.iteration++;

	m_timeSinceLastUpdate = 0.f;

	resetTickData();
	updateSubSystems();
	invokeEvents();
}

auto SimRunnerSystem::resetTickData() -> void
{
	auto& tickData = m_registry.ctx().get< TickDataCollection >();
	tickData       = TickDataCollection{};
}

auto SimRunnerSystem::updateSubSystems() -> void
{
	const auto timeStamp = getTimeStamp();

	for ( auto& system : m_simSystems )
	{
		system->update();
	}

	const auto updateDuration = getTimeStampDiff( timeStamp );
	auto& data                = m_registry.ctx().get< SimRunnerData >();
	data.tickTime             = updateDuration;
}

auto SimRunnerSystem::invokeEvents() -> void
{
	auto& dispatcher   = m_registry.ctx().get< entt::dispatcher >();
	auto& data         = m_registry.ctx().get< SimRunnerData >();
	const auto& grid   = m_registry.ctx().get< Grid >();
	const auto& preset = m_registry.ctx().get< Preset >();

	if ( data.iteration >= preset.iterationTarget )
	{
		if ( !data.targetReached )
		{
			data.targetReached = true;
			data.paused        = true;

			dispatcher.trigger< event::ReachedTargetIteration >();
			if ( !m_limitSpeed ) m_blockExecution = true;
		}
	}
	else if ( grid.getPopulation() == 0uz )
	{
		dispatcher.enqueue< event::Extinction >( data.iteration );
		if ( !m_limitSpeed ) m_blockExecution = true;
	}
}

auto SimRunnerSystem::shouldUpdate() -> bool
{
	auto& data = m_registry.ctx().get< SimRunnerData >();
	auto& time = m_registry.ctx().get< Time >();

	if ( data.targetReached && !m_limitSpeed )
	{
		return false;
	}

	const float timeBetweenUpdates = 1.f / static_cast< float >( data.speed );
	if ( data.paused )
	{
		m_timeSinceLastUpdate = timeBetweenUpdates;
		return false;
	}

	const bool timeToUpdate = m_timeSinceLastUpdate > timeBetweenUpdates;
	if ( !timeToUpdate )
	{
		m_timeSinceLastUpdate += time.deltaTime;
		return false;
	}

	return true;
}

auto SimRunnerSystem::onResetSim( const event::ResetSim& /*event*/ ) -> void
{
	auto& runnerData = m_registry.ctx().get< SimRunnerData >();
	runnerData       = SimRunnerData{};

	auto& random = m_registry.ctx().get< Randomizer >();
	random.reset();

	m_registry.clear();

	const auto gridArgs = m_registry.ctx().get< Grid >().copyCreationArguments();
	m_registry.ctx().erase< Grid >();
	m_registry.ctx().emplace< Grid >( gridArgs );

	const float timeBetweenUpdates = 1.f / static_cast< float >( runnerData.speed );
	m_timeSinceLastUpdate          = timeBetweenUpdates;
}
}  // namespace cc::app