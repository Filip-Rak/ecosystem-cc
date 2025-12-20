#include "Application/ContextEntity/Grid.hpp"

#include <cstddef>

#include <entt/entt.hpp>

#include "Application/Components/Agent.hpp"
#include "Application/Components/GeneSet.hpp"

namespace cc::app
{
namespace
{
auto initSpacialGrid( entt::registry& registry, std::vector< std::vector< entt::entity > >& spatialGrid ) -> void
{
	// TODO: const auto& basicGenes = registry.get</*preset::Genes*/>;
	const auto initialGenes   = component::GeneSet::Genes{ .maxEnergy = 100.f, .perception = 2uz };
	const auto initialGeneSet = component::GeneSet{ .agentGenes = initialGenes, .futureGenes = initialGenes };

	for ( auto index{ 0uz }; index < spatialGrid.size(); index++ )
	{
		auto& spatialCell  = spatialGrid[ index ];
		const auto& entity = spatialCell.emplace_back( registry.create() );

		registry.emplace< component::GeneSet >( entity, initialGeneSet );

		auto& agent     = registry.emplace< component::Agent >( entity );
		agent.energy    = initialGenes.maxEnergy;
		agent.cellIndex = index;
	}
}
}  // namespace

Grid::Grid( entt::registry& registry, uint16_t width, uint16_t height )
    : m_width( width ),
      m_height( height ),
      m_cellSize( static_cast< std::size_t >( width ) * height ),
      m_signedCellSize( static_cast< std::ptrdiff_t >( width ) * height )
{
	const auto cellCount = static_cast< std::size_t >( m_signedCellSize );
	m_cells.reserve( cellCount );

	m_spatialGrid.resize( cellCount );
	initSpacialGrid( registry, m_spatialGrid );
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

auto Grid::getSignedCellSize() const -> uint16_t
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
}  // namespace cc::app