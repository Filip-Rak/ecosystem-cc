#include "Application/System/Sim/AgentDecisionSystem.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>

#include <entt/entt.hpp>

#include "Application/Components/EatIntent.hpp"
#include "Application/Components/FailedToMate.hpp"
#include "Application/Components/GeneSet.hpp"
#include "Application/Components/MoveIntent.hpp"
#include "Application/Components/OffspringIntent.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Cell.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
namespace
{
auto rangeOffsets( const Grid& grid, std::size_t range ) -> std::vector< std::ptrdiff_t >
{
	std::vector< std::ptrdiff_t > offsets;
	const auto side = ( 2uz * range ) + 1uz;
	offsets.reserve( side * side );

	const auto signedRange = static_cast< std::ptrdiff_t >( range );
	for ( auto deltaY = -signedRange; deltaY <= signedRange; deltaY++ )
	{
		for ( auto deltaX = -signedRange; deltaX <= signedRange; deltaX++ )
		{
			const auto index = grid.PositionToIndex( deltaX, deltaY );
			offsets.emplace_back( index );
		}
	}

	return offsets;
}

auto nextStepUnsafe( const Grid& grid, std::size_t startIndex, std::size_t targetIndex ) -> std::size_t
{
	auto startPosition        = grid.indexToPosition( startIndex );
	const auto targetPosition = grid.indexToPosition( targetIndex );

	const auto deltaX = targetPosition.x - startPosition.x;
	const auto deltaY = targetPosition.y - startPosition.y;

	if ( deltaX != 0 ) startPosition.x += ( deltaX > 0 ) ? 1 : -1;
	if ( deltaY != 0 ) startPosition.y += ( deltaY > 0 ) ? 1 : -1;

	return grid.PositionToIndex( startPosition );
}

auto getEnergyCost( const component::Vitals& vitals, const Genes& agentGenes, const Cell& cell, const Preset& preset,
                    float baseCost ) -> float
{
	const float tDiff = std::abs( cell.temperature - agentGenes.temperaturePreference );
	const float eDiff = std::abs( cell.elevation - agentGenes.elevationPreference );
	const float hDiff = std::abs( cell.humidity - agentGenes.humidityPreference );

	const auto& sensitivity = preset.agent.environmentalSensitivity;
	const float base        = sensitivity.basePower;

	const float tPenalty = std::pow( tDiff, base * sensitivity.temperaturePower );
	const float ePenalty = std::pow( eDiff, base * sensitivity.elevationPower );
	const float hPenalty = std::pow( hDiff, base * sensitivity.humidityPower );

	const float combinedPenalty = tPenalty + ePenalty + hPenalty;

	const int refractory = agentGenes.refractoryPeriod;
	assert( refractory > 0 );

	const float underageFactor =
	    ( vitals.age < refractory )
	        ? sensitivity.youthPayFactor + ( sensitivity.acclimationIncrement / static_cast< float >( refractory ) )
	        : 1.f;

	const float finalCost = combinedPenalty * underageFactor * baseCost;
	return std::min( finalCost, agentGenes.maxEnergy * sensitivity.maxDrainCap );
}

auto calcMoveCost( const component::Vitals& vitals, const Grid& grid, const Genes& agentGenes, const Preset& preset,
                   const std::size_t startPos, const std::size_t targetPos, float baseCost ) -> float
{
	float totalCost   = 0.f;
	const auto& cells = grid.cells();

	std::size_t currentPos = startPos;
	while ( currentPos != targetPos )
	{
		currentPos = nextStepUnsafe( grid, currentPos, targetPos );
		totalCost += getEnergyCost( vitals, agentGenes, cells[ currentPos ], preset, baseCost );
	}

	return totalCost;
}

auto bestCell( std::vector< std::uint8_t >& moveIntentions, const Grid& grid, const Genes& genes,
               const component::Vitals& vitals, const Preset& preset,
               const std::vector< std::vector< std::ptrdiff_t > >& rangeOffsets, const std::size_t cellIndex )
    -> std::size_t
{
	const auto& cells         = grid.cells();
	const auto& spatialCells  = grid.getSpatialGrid();
	const auto perception     = static_cast< std::uint8_t >( genes.perception );
	const auto startSigned    = static_cast< std::ptrdiff_t >( cellIndex );
	const float crowdPenalty  = preset.agent.environmentalSensitivity.crowdPenalty;
	const auto energyFactor   = vitals.energy / genes.maxEnergy;
	const auto& modifiers     = preset.agent.modifier;
	const auto traversalCost  = modifiers.baseTraversalCost;
	const auto baseEnergyBurn = modifiers.baseEnergyBurn;
	const auto maxIntake      = modifiers.maxIntake;

	assert( cellIndex < cells.size() );

	float bestScore      = -std::numeric_limits< float >::infinity();
	std::size_t bestCell = cellIndex;

	for ( const auto offset : rangeOffsets[ perception - 1 ] )
	{
		// Clip at borders.
		const auto newIndex = startSigned + offset;
		if ( newIndex >= grid.getSignedCellCount() || newIndex < 0 || newIndex == startSigned ) continue;

		const auto newIndexUnsigned = static_cast< std::size_t >( newIndex );
		const auto& cell            = cells[ newIndexUnsigned ];

		const auto crowdCurrent = spatialCells[ newIndexUnsigned ].size();
		const auto crowdFuture  = moveIntentions[ newIndexUnsigned ];
		const auto crowdMax     = crowdCurrent + crowdFuture;
		const float crowdScore  = static_cast< float >( crowdMax ) * crowdPenalty;

		const auto food      = cell.getCombinedFoodGain( genes );
		const float moveCost = calcMoveCost( vitals, grid, genes, preset, cellIndex, newIndexUnsigned, traversalCost );
		const float sustainScore = food / getEnergyCost( vitals, genes, cell, preset, baseEnergyBurn );

		const float score = sustainScore - crowdScore - moveCost;
		if ( score > bestScore )
		{
			bestScore = score;
			bestCell  = newIndexUnsigned;
		}
	}

	return bestCell;
}

auto averageSustainAround( const component::Vitals& vitals, const Genes& agentGenes, const Grid& grid,
                           const Preset& preset, const std::vector< std::vector< std::ptrdiff_t > >& rangeOffsets,
                           std::size_t centerIndex ) -> float
{
	const auto& cells       = grid.cells();
	const auto& spatialGrid = grid.getSpatialGrid();
	const auto perception   = agentGenes.perception;

	const auto centerIndexSigned = static_cast< std::ptrdiff_t >( centerIndex );
	const auto baseCost          = preset.agent.modifier.baseTraversalCost;

	float sustain    = 0.f;
	float validCells = 0.f;

	for ( const auto offset : rangeOffsets[ perception - 1 ] )
	{
		// Clip at borders.
		const auto newIndex = centerIndexSigned + offset;
		if ( newIndex >= grid.getSignedCellCount() || newIndex < 0 ) continue;

		validCells += 1.f;

		const auto& cell = cells[ newIndex ];

		const auto population = spatialGrid[ newIndex ].size();
		const auto energyCost = getEnergyCost( vitals, agentGenes, cell, preset, baseCost );

		const auto sustainAddition = ( cell.getCombinedFoodGain( agentGenes ) ) / energyCost;
		sustain += ( population > 0 ) ? sustainAddition / static_cast< float >( population ) : sustainAddition;
	}

	const float avgSustain = sustain / validCells;
	return avgSustain;
}

enum class Action : std::uint8_t
{
	Flee,
	Eat,
	Mate,
	ExploreStarving,
	ExploreFull
};

auto getAction( entt::registry& registry, entt::entity entity,
                const std::vector< std::vector< std::ptrdiff_t > >& rangeOffsets ) -> Action
{
	const auto& grid   = registry.ctx().get< Grid >();
	const auto& preset = registry.ctx().get< Preset >();

	const auto index      = registry.get< component::Position >( entity ).cellIndex;
	const auto population = grid.getSpatialGrid()[ index ].size();
	const auto& genes     = registry.get< component::GeneSet >( entity ).agentGenes;
	const auto& vitals    = registry.get< component::Vitals >( entity );

	const auto& cell = grid.cells()[ index ];

	constexpr bool inDanger = false;
	if ( inDanger )
	{
		return Action::Flee;
	}

	const float fullnessFactor           = vitals.energy / genes.maxEnergy;
	const float offspringRequiredFulness = preset.agent.modifier.offspringRequiredFullness;
	const bool canBearOffspring = vitals.remainingRefractoryPeriod <= 0 && fullnessFactor >= offspringRequiredFulness;

	if ( canBearOffspring )
	{
		const float threshold  = preset.agent.environmentalSensitivity.offspringSustainmentNeed;
		const float avgSustain = averageSustainAround( vitals, genes, grid, preset, rangeOffsets, index );

		if ( avgSustain > threshold )
		{
			return Action::Mate;
		}

		registry.emplace< component::FailedToMate >( entity );
		return Action::ExploreFull;
	}
	if ( fullnessFactor == 1.f )
	{
		return Action::ExploreFull;
	}

	const float energyCost      = getEnergyCost( vitals, genes, cell, preset, preset.agent.modifier.baseEnergyBurn );
	const float combinedSustain = cell.getCombinedFoodGain( genes );
	const float sustainabilityFactor = combinedSustain / energyCost / static_cast< float >( population );
	const bool unsustainable         = energyCost > preset.agent.modifier.maxIntake || sustainabilityFactor < 1.f;

	if ( unsustainable )
	{
		return Action::ExploreStarving;
	}

	return Action::Eat;
}

auto getPerception( entt::registry& registry ) -> std::size_t
{
	assert( registry.ctx().contains< Preset >() );
	return registry.ctx().get< Preset >().agent.modifier.maxPerception;
}
}  // namespace

AgentDecisionSystem::AgentDecisionSystem( entt::registry& registry )
    : m_registry( registry ), m_maxPerception( getPerception( registry ) )
{
	assert( registry.ctx().contains< Grid >() );
	auto& grid = registry.ctx().get< Grid >();

	m_moveIntentions.resize( grid.getCellCount() );

	for ( auto range{ 0uz }; range < m_maxPerception; range++ )
	{
		m_rangeOffsets.emplace_back( rangeOffsets( grid, range ) );
	}
}

auto AgentDecisionSystem::update() -> void
{
	const auto& preset      = m_registry.ctx().get< Preset >();
	auto& grid              = m_registry.ctx().get< Grid >();
	const auto& spatialGrid = grid.getSpatialGrid();
	const auto& cells       = grid.cells();

	std::ranges::fill( m_moveIntentions, std::uint8_t{ 0 } );

	const auto view = m_registry.view< const component::Position, const component::GeneSet, component::Vitals >();
	for ( const auto& [ entity, position, geneSet, vitals ] : view.each() )
	{
		const auto& cell = cells[ position.cellIndex ];

		const auto population = static_cast< float >( spatialGrid[ position.cellIndex ].size() );
		const Action action   = getAction( m_registry, entity, m_rangeOffsets );

		using enum Action;
		if ( action == Mate )
		{
			m_registry.emplace< component::OffspringIntent >( entity );
			m_moveIntentions[ position.cellIndex ]++;
		}
		else if ( action == Eat )
		{
			m_registry.emplace< component::EatIntent >( entity );
			m_moveIntentions[ position.cellIndex ]++;
		}
		else if ( action == ExploreStarving || action == ExploreFull )
		{
			const std::size_t betterCell = bestCell( m_moveIntentions, grid, geneSet.agentGenes, vitals, preset,
			                                         m_rangeOffsets, position.cellIndex );

			assert( betterCell != position.cellIndex );
			const std::size_t nextCellIndex = nextStepUnsafe( grid, position.cellIndex, betterCell );
			const auto nextCell             = cells[ nextCellIndex ];
			const auto traverseCost         = preset.agent.modifier.baseTraversalCost;

			const float moveCost = getEnergyCost( vitals, geneSet.agentGenes, nextCell, preset, traverseCost );

			m_registry.emplace< component::MoveIntent >( entity, nextCellIndex, moveCost );
			m_moveIntentions[ nextCellIndex ]++;
		}
		else if ( action == Flee )
		{}
	}
}
}  // namespace cc::app