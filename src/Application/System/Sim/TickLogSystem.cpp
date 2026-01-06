#include "Application/System/Sim/TickLogSystem.hpp"

#include <cassert>

#include <cstddef>
#include <entt/entt.hpp>
#include <vector>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Logger.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/ContextEntity/TickDataCollection.hpp"
#include "Application/ContextEntity/TickLog.hpp"
#include "entt/entity/fwd.hpp"

namespace cc::app
{
namespace
{
auto quantile_nearest( std::vector< float > values, float quantile ) -> float
{
	if ( values.empty() ) return 0.f;
	quantile = std::clamp( quantile, 0.f, 1.f );

	const auto index = static_cast< size_t >( quantile * ( static_cast< float >( values.size() - 1 ) ) );
	std::nth_element( values.begin(), values.begin() + static_cast< ptrdiff_t >( index ), values.end() );

	return values[ index ];
}

auto logFromFrameData( entt::registry& registry, TickLog& tickLog ) -> void
{
	const auto& tickData   = registry.ctx().get< TickDataCollection >();
	const auto& runnerData = registry.ctx().get< SimRunnerData >();

	tickLog.iteration       = runnerData.iteration;
	tickLog.births          = tickData.births;
	tickLog.starvations     = tickData.starvations;
	tickLog.ageDeaths       = tickData.oldAgeDeaths;
	tickLog.vegetationEaten = tickData.vegetationEaten;
	tickLog.moves           = tickData.moves;
}

auto logFromCells( const Grid& grid, TickLog& tickLog ) -> void
{
	float vegetationSum = 0.f;

	const auto& cells = grid.cells();
	for ( const auto& cell : cells )
	{
		vegetationSum += cell.vegetation;
	}

	tickLog.vegetationTotal = vegetationSum;
	tickLog.meanVegetation  = vegetationSum / static_cast< float >( grid.getCellCount() );

	auto crowdingSum   = 0uz;
	auto occupiedCells = 0uz;

	const auto& spatialCells = grid.getSpatialGrid();
	for ( const auto& cell : spatialCells )
	{
		if ( cell.size() > 0 )
		{
			crowdingSum += cell.size();
			occupiedCells++;
		}
	}

	tickLog.occupiedCells = occupiedCells;
	tickLog.meanCrowd     = static_cast< float >( crowdingSum ) / static_cast< float >( occupiedCells );
}
}  // namespace

TickLogSystem::TickLogSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< std::unique_ptr< Logger > >() );
	assert( registry.ctx().contains< TickDataCollection >() );
	assert( registry.ctx().contains< SimRunnerData >() );
	assert( registry.ctx().contains< Grid >() );

	constexpr auto prealloc = 1000uz;
	m_agentEnergy.reserve( prealloc );
	m_agentEnergyGene.reserve( prealloc );
}

auto TickLogSystem::update() -> void
{
	auto& logger     = m_registry.ctx().get< std::unique_ptr< Logger > >();
	const auto& grid = m_registry.ctx().get< Grid >();

	TickLog tickLog;
	logFromFrameData( m_registry, tickLog );
	logFromAgents( grid, tickLog );
	logFromCells( grid, tickLog );

	logger->writeTickData( tickLog );
}

auto TickLogSystem::logFromAgents( const Grid& grid, TickLog& tickLog ) -> void
{
	const auto& cells = grid.cells();
	m_agentEnergyGene.clear();
	m_agentEnergy.clear();

	auto agentCount           = 0uz;
	float energyGeneSum       = 0.f;
	float energyGeneSqSum     = 0.f;
	float energySum           = 0.f;
	float energySqSum         = 0.f;
	float tempAdaptSum        = 0.f;
	float humAdaptSum         = 0.f;
	float elevAdaptSum        = 0.f;
	float adaptationRatingSum = 0.f;

	const auto& view =
	    m_registry.view< const component::GeneSet, const component::Vitals, const component::Position >();
	for ( const auto& [ entity, geneSet, vitals, position ] : view.each() )
	{
		const auto& genes = geneSet.agentGenes;
		const auto& cell  = cells[ position.cellIndex ];

		agentCount++;
		energyGeneSum += genes.maxEnergy;
		energyGeneSqSum += genes.maxEnergy * genes.maxEnergy;
		energySum += vitals.energy;
		energySqSum += vitals.energy * vitals.energy;

		m_agentEnergy.push_back( vitals.energy );
		m_agentEnergyGene.push_back( genes.maxEnergy );

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

		tickLog.energyVar     = ( energySqSum / fAgentCount ) - ( tickLog.meanEnergy * tickLog.meanEnergy );
		tickLog.energyGeneVar = ( energyGeneSqSum / fAgentCount ) - ( tickLog.meanEnergyGene * tickLog.meanEnergyGene );
	}

	constexpr auto p50    = 0.5f;
	constexpr auto p90    = 0.9f;
	tickLog.energyP50     = quantile_nearest( m_agentEnergy, p50 );
	tickLog.energyP90     = quantile_nearest( m_agentEnergy, p90 );
	tickLog.energyGeneP50 = quantile_nearest( m_agentEnergyGene, p50 );
	tickLog.energyGeneP90 = quantile_nearest( m_agentEnergyGene, p90 );
}
}  // namespace cc::app