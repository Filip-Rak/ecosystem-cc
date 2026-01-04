#include "Application/System/Sim/TickLogSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Logger.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/ContextEntity/TickDataCollection.hpp"
#include "Application/ContextEntity/TickLog.hpp"

namespace cc::app
{
TickLogSystem::TickLogSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< std::unique_ptr< Logger > >() );
	assert( registry.ctx().contains< TickDataCollection >() );
	assert( registry.ctx().contains< SimRunnerData >() );
}

auto TickLogSystem::update() -> void
{
	TickLog tickLog;

	auto& logger           = m_registry.ctx().get< std::unique_ptr< Logger > >();
	const auto& tickData   = m_registry.ctx().get< TickDataCollection >();
	const auto& runnerData = m_registry.ctx().get< SimRunnerData >();

	tickLog.iteration       = runnerData.iteration;
	tickLog.births          = tickData.births;
	tickLog.starvations     = tickData.starvations;
	tickLog.ageDeaths       = tickData.oldAgeDeaths;
	tickLog.vegetationEaten = tickData.vegetationEaten;

	auto agentCount           = 0uz;
	float energyGeneSum       = 0.f;
	float energySum           = 0.f;
	float tempAdaptSum        = 0.f;
	float humAdaptSum         = 0.f;
	float elevAdaptSum        = 0.f;
	float adaptationRatingSum = 0.f;

	const auto& grid  = m_registry.ctx().get< Grid >();
	const auto& cells = grid.cells();

	const auto& view =
	    m_registry.view< const component::GeneSet, const component::Vitals, const component::Position >();
	for ( const auto& [ entity, geneSet, vitals, position ] : view.each() )
	{
		const auto& genes = geneSet.agentGenes;
		const auto& cell  = cells[ position.cellIndex ];

		agentCount++;
		energySum += vitals.energy;
		energyGeneSum += genes.maxEnergy;

		const float tDiff = std::abs( cell.temperature - genes.temperaturePreference );
		tempAdaptSum += 1.f - tDiff;

		const float hDiff = std::abs( cell.humidity - genes.humidityPreference );
		humAdaptSum += 1.f - hDiff;

		const float eDiff = std::abs( cell.elevation - genes.elevationPreference );
		elevAdaptSum += 1.f - eDiff;

		const float adaptation = 1 - ( ( tDiff + hDiff + eDiff ) / 3.f );
		adaptationRatingSum += adaptation;
	}

	if ( agentCount > 0uz )
	{
		const auto fAgentCount     = static_cast< float >( agentCount );
		tickLog.liveAgents         = agentCount;
		tickLog.meanEnergyGene     = energyGeneSum / fAgentCount;
		tickLog.meanEnergy         = energySum / fAgentCount;
		tickLog.meanTempAdaptation = tempAdaptSum / fAgentCount;
		tickLog.meanHumAdaptation  = humAdaptSum / fAgentCount;
		tickLog.meanElevAdaptation = elevAdaptSum / fAgentCount;
	}

	logger->logTickData( tickLog );
}
}  // namespace cc::app