#include "Application/System/Sim/AgentDecisionSystem.hpp"

#include <cassert>
#include <cstddef>
#include <limits>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/MoveIntent.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Cell.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
namespace
{
// TODO: Cheybyshev - consider Manhattan for better performance.
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

// TODO: Cheybyshev - consider Manhattan for better performance.
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

auto getEnergyCost( const Genes& agentGenes, const Cell& cell, float baseCost ) -> float
{
	const float tempCost = std::abs( cell.temperature - agentGenes.temperaturePreference );
	const float elevCost = std::abs( cell.elevation - agentGenes.elevationPreference );
	const float humCost  = std::abs( cell.humidity - agentGenes.humidityPreference );

	return ( tempCost + elevCost + humCost ) * baseCost;
}

auto calcMoveCost( const Grid& grid, const Genes& agentGenes, const std::size_t startPos, const std::size_t targetPos,
                   float baseCost ) -> float
{
	float totalCost   = 0.f;
	const auto& cells = grid.getCells();

	std::size_t currentPos = startPos;
	while ( currentPos != targetPos )
	{
		currentPos = nextStepUnsafe( grid, currentPos, targetPos );
		totalCost += getEnergyCost( agentGenes, cells[ currentPos ], baseCost );
	}

	return totalCost;
}

auto bestCell( const Grid& grid, const Genes& agentGenes, const component::Vitals& vitals, const Preset& preset,
               const std::vector< std::vector< std::ptrdiff_t > >& rangeOffsets, const std::size_t cellIndex )
    -> std::size_t
{
	const auto& cells              = grid.getCells();
	const auto& spatialCells       = grid.getSpatialGrid();
	const auto perception          = agentGenes.perception;
	const auto startingCell        = static_cast< std::ptrdiff_t >( cellIndex );
	const auto energyDeficitFactor = agentGenes.maxEnergy - ( vitals.energy / agentGenes.maxEnergy );

	assert( cellIndex < cells.size() );

	std::size_t bestCell = cellIndex;
	float bestScore      = -std::numeric_limits< float >::infinity();

	for ( const auto offset : rangeOffsets[ perception - 1 ] )
	{
		// Clip at borders
		const auto newIndex = startingCell + offset;
		if ( newIndex >= grid.getSignedCellCount() || newIndex < 0 ) continue;

		const auto newIndexUnsigned = static_cast< std::size_t >( newIndex );
		const auto& cell            = cells[ newIndexUnsigned ];

		const auto food  = cell.vegetation;
		const auto crowd = spatialCells[ newIndexUnsigned ].size();
		const auto moveCost =
		    calcMoveCost( grid, agentGenes, cellIndex, newIndexUnsigned, preset.agent.modifier.baseTraversalCost );

		constexpr float crowdPenalty = 0.05f;
		const float crowdScore       = static_cast< float >( crowd ) * crowdPenalty;
		const float foodScore        = food * energyDeficitFactor;

		const float score = foodScore - crowdScore - moveCost;
		if ( score > bestScore )
		{
			bestScore = score;
			bestCell  = newIndexUnsigned;
		}
	}

	return bestCell;
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

	for ( auto range = 0uz; range < m_maxPerception; range++ )
	{
		m_rangeOffsets.emplace_back( rangeOffsets( grid, range ) );
	}
}

// TODO: Signed, unsigned is a mess. Figure out if full switch to signed is feasible or minimize
auto AgentDecisionSystem::update() -> void
{
	auto& grid         = m_registry.ctx().get< Grid >();
	const auto& preset = m_registry.ctx().get< Preset >();

	const auto view = m_registry.view< const component::Position, const component::GeneSet, component::Vitals >();
	for ( const auto& [ entity, position, geneSet, vitals ] : view.each() )
	{
		// TODO: Move to a different system
		constexpr float energyTax = 0.1f;
		vitals.energy -= energyTax;

		assert( geneSet.agentGenes.perception <= m_maxPerception );

		const auto bestIndex = bestCell( grid, geneSet.agentGenes, vitals, preset, m_rangeOffsets, position.cellIndex );
		if ( position.cellIndex != bestIndex )
		{
			const auto stepIndex = nextStepUnsafe( grid, position.cellIndex, bestIndex );
			m_registry.emplace_or_replace< component::MoveIntent >( entity, stepIndex );
		}
	}
}
}  // namespace cc::app