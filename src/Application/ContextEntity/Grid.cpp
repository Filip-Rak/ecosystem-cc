#include "Application/ContextEntity/Grid.hpp"

#include <cassert>
#include <cstddef>
#include <limits>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"

namespace cc::app
{
Grid::Grid( entt::registry& registry, uint16_t width, uint16_t height )
    : m_width( width ),
      m_height( height ),
      m_cellSize( static_cast< std::size_t >( width ) * height ),
      m_signedCellSize( static_cast< std::ptrdiff_t >( width ) * height ),
      m_registry( registry )
{
	const auto cellCount = static_cast< std::size_t >( m_signedCellSize );
	m_spatialGrid.resize( cellCount );
	m_cells.reserve( cellCount );

	const auto initialGenes   = component::GeneSet::Genes{ .maxEnergy = 100.f, .perception = 2uz };
	const auto initialGeneSet = component::GeneSet{ .agentGenes = initialGenes, .futureGenes = initialGenes };

	for ( auto index{ 0uz }; index < m_spatialGrid.size(); index++ )
	{
		auto& spatialCell  = m_spatialGrid[ index ];
		const auto& entity = registry.create();

		registry.emplace< component::GeneSet >( entity, initialGeneSet );
		registry.emplace< component::Position >( entity );

		spatialCell.reserve( 4 );
		addToSpatialGrid( entity, index );
	}
}

void Grid::moveEntity( entt::entity entity, std::size_t currentCell, std::size_t targetCell )
{
	assert( currentCell != targetCell );
	removeFromSpatialGrid( entity, currentCell );
	addToSpatialGrid( entity, targetCell );
}

auto Grid::getWidth() const -> uint16_t
{
	return m_width;
}

auto Grid::getHeight() const -> uint16_t
{
	return m_height;
}

auto Grid::getCellSize() const -> std::size_t
{
	return m_cellSize;
}

auto Grid::getSignedCellSize() const -> std::ptrdiff_t
{
	return m_signedCellSize;
}

auto Grid::getCells() const -> const std::vector< Cell >&
{
	return m_cells;
}

auto Grid::getSpatialGrid() const -> const SpatialGrid&
{
	return m_spatialGrid;
}

auto Grid::cells() -> std::vector< Cell >&
{
	return m_cells;
}

void Grid::addToSpatialGrid( entt::entity entity, std::size_t cellIndex )
{
	auto& bucket   = m_spatialGrid[ cellIndex ];
	auto& position = m_registry.get< component::Position >( entity );

	position.spatialIndex = bucket.size();
	position.cellIndex    = cellIndex;
	bucket.push_back( entity );
}

void Grid::removeFromSpatialGrid( entt::entity targetEntity, std::size_t cellIndex )
{
	auto& bucket      = m_spatialGrid[ cellIndex ];
	auto& targetIndex = m_registry.get< component::Position >( targetEntity ).spatialIndex;

	// FIXME: Will fail after restart - fix restart to reset buckets.
	assert( targetIndex < bucket.size() );
	assert( bucket[ targetIndex ] == targetEntity );

	const auto lastEntity = bucket.back();
	bucket[ targetIndex ] = lastEntity;
	bucket.pop_back();

	if ( targetEntity != lastEntity )
	{
		auto& lastPosition        = m_registry.get< component::Position >( lastEntity );
		lastPosition.spatialIndex = targetIndex;
	}

	targetIndex = std::numeric_limits< std::size_t >::max();
}
}  // namespace cc::app