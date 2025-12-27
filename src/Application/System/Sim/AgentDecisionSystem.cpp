#include "Application/System/Sim/AgentDecisionSystem.hpp"

#include <cassert>
#include <cstddef>

#include <entt/entt.hpp>

#include "Application/Components/Destroy.hpp"
#include "Application/Components/EatIntent.hpp"
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
constexpr float refractorEpsilon = 0.05f;

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

auto getSatietyCost( const Genes& agentGenes, const Cell& cell, float baseCost ) -> float
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
	const auto& cells = grid.cells();

	std::size_t currentPos = startPos;
	while ( currentPos != targetPos )
	{
		currentPos = nextStepUnsafe( grid, currentPos, targetPos );
		totalCost += getSatietyCost( agentGenes, cells[ currentPos ], baseCost );
	}

	return totalCost;
}

auto getCurrentCellScore( const Cell& currentCell, const Genes& genes, std::size_t refractoryLeft, float satietyFactor,
                          float metabolism, float maxIntake, float satiety ) -> float
{
	const float cost = getSatietyCost( genes, currentCell, metabolism );
	if ( cost > maxIntake ) return 0.f;

	const float currentSustain      = currentCell.vegetation / cost;
	const float currentSustainScore = ( currentSustain < 1.f ) ? 0.f : currentSustain * ( 1 - satietyFactor );
	const float refractorScore =
	    ( std::abs( satiety - genes.maxSatiety ) < refractorEpsilon && refractoryLeft <= 0 ) ? 20.f : 0.f;

	return currentSustainScore + refractorScore;
}

auto bestCell( const Grid& grid, const Genes& genes, const component::Vitals& vitals, const Preset& preset,
               const std::vector< std::vector< std::ptrdiff_t > >& rangeOffsets, const std::size_t cellIndex )
    -> std::size_t
{
	const auto& cells        = grid.cells();
	const auto& spatialCells = grid.getSpatialGrid();
	const auto perception    = genes.perception;
	const auto startSigned   = static_cast< std::ptrdiff_t >( cellIndex );
	const auto traversalCost = preset.agent.modifier.baseTraversalCost;
	const auto metabolism    = preset.agent.modifier.metabolism;
	const auto satietyFactor = vitals.satiety / genes.maxSatiety;
	const auto maxIntake     = preset.agent.modifier.maxIntake;

	assert( cellIndex < cells.size() );

	// Rate current cell
	float bestScore = getCurrentCellScore( cells[ cellIndex ], genes, vitals.remainingRefractoryPeriod, satietyFactor,
	                                       metabolism, maxIntake, vitals.satiety );
	std::size_t bestCell = cellIndex;

	for ( const auto offset : rangeOffsets[ perception - 1 ] )
	{
		// Clip at borders.
		const auto newIndex = startSigned + offset;
		if ( newIndex >= grid.getSignedCellCount() || newIndex < 0 || newIndex == startSigned ) continue;

		const auto newIndexUnsigned = static_cast< std::size_t >( newIndex );
		const auto& cell            = cells[ newIndexUnsigned ];

		const auto crowd             = spatialCells[ newIndexUnsigned ].size();
		constexpr float crowdPenalty = 0.5f;
		const float crowdScore       = static_cast< float >( crowd ) * crowdPenalty;

		const auto food          = cell.vegetation;
		const float moveCost     = calcMoveCost( grid, genes, cellIndex, newIndexUnsigned, traversalCost );
		const float sustain      = food / getSatietyCost( genes, cell, metabolism );
		const float sustainScore = sustain * std::pow( satietyFactor, 4.f );

		const float score = sustainScore - crowdScore - moveCost;
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
	const auto& cells  = grid.cells();
	const auto& preset = m_registry.ctx().get< Preset >();

	const auto view = m_registry.view< const component::Position, const component::GeneSet, component::Vitals >();
	for ( const auto& [ entity, position, geneSet, vitals ] : view.each() )
	{
		const auto& genes = geneSet.agentGenes;
		assert( genes.perception <= m_maxPerception );

		vitals.remainingRefractoryPeriod -= 1;
		if ( vitals.satiety <= 0.f )
		{
			m_registry.emplace< component::Destroy >( entity, component::Destroy::Reason::Starvation );
			continue;
		}

		const auto result = bestCell( grid, genes, vitals, preset, m_rangeOffsets, position.cellIndex );
		if ( position.cellIndex == result )
		{
			if ( std::abs( vitals.satiety - genes.maxSatiety ) < refractorEpsilon &&
			     vitals.remainingRefractoryPeriod <= 0 )
			{
				m_registry.emplace< component::OffspringIntent >( entity );
			}
			else
			{
				m_registry.emplace< component::EatIntent >( entity );

				const auto& cell       = cells[ position.cellIndex ];
				const auto& metabolism = preset.agent.modifier.metabolism;
				vitals.satiety -= getSatietyCost( genes, cell, metabolism );
			}
		}
		else
		{
			const auto stepIndex = nextStepUnsafe( grid, position.cellIndex, result );
			m_registry.emplace< component::MoveIntent >( entity, stepIndex,
			                                             getSatietyCost( geneSet.agentGenes, grid.cells()[ stepIndex ],
			                                                             preset.agent.modifier.baseTraversalCost ) );
		}
	}
}
}  // namespace cc::app