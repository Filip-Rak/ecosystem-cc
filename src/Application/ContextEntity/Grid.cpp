#include "Application/ContextEntity/Grid.hpp"

#include <cassert>
#include <cstddef>
#include <limits>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
Grid::Grid( const Args& args )
    : m_creationArguments( args ),
      m_width( args.width ),
      m_height( args.height ),
      m_cellCount( static_cast< std::size_t >( args.width ) * args.height ),
      m_signedCellCount( static_cast< std::ptrdiff_t >( args.width ) * args.height ),
      m_registry( args.registry )
{
	m_spatialGrid.resize( m_cellCount );
	m_cells.reserve( m_cellCount );

	constexpr auto initialGenes   = component::GeneSet::Genes{};
	constexpr auto initialGeneSet = component::GeneSet{ .agentGenes = initialGenes, .futureGenes = initialGenes };
	constexpr auto initialEnergy  = initialGeneSet.agentGenes.maxEnergy;

	for ( auto index{ 0uz }; index < m_spatialGrid.size(); index++ )
	{
		auto& spatialCell  = m_spatialGrid[ index ];
		const auto& entity = m_registry.create();

		m_registry.emplace< component::GeneSet >( entity, initialGeneSet );
		m_registry.emplace< component::Vitals >( entity, initialEnergy );
		m_registry.emplace< component::Position >( entity );

		constexpr auto preallocatedEntities = 4uz;
		spatialCell.reserve( preallocatedEntities );
		addToSpatialGrid( entity, index );
	}

	const Preset::Vegetation& vegetationPreset = m_registry.ctx().get< Preset >().vegetation;
	for ( std::size_t index = 0; index < m_cellCount; index++ )
	{
		const float cellTemperature = args.temperatureValues[ index ];
		const float cellHumidity    = args.humidityValues[ index ];
		const float cellElevation   = args.elevationValues[ index ];

		m_cells.emplace_back( 0.f, cellTemperature, cellElevation, cellHumidity, vegetationPreset );
	}
}

void Grid::moveEntity( entt::entity entity, std::size_t currentCell, std::size_t targetCell )
{
	assert( currentCell != targetCell );
	removeFromSpatialGrid( entity, currentCell );
	addToSpatialGrid( entity, targetCell );
}

auto Grid::indexToPosition( std::size_t index ) const -> Position
{
	return { .x = static_cast< ptrdiff_t >( index % m_width ), .y = static_cast< ptrdiff_t >( index / m_width ) };
}

auto Grid::PositionToIndex( ptrdiff_t x, ptrdiff_t y ) const -> std::size_t
{
	return ( y * m_width ) + x;
}

auto Grid::PositionToIndex( Position position ) const -> std::size_t
{
	return PositionToIndex( position.x, position.y );
}

auto Grid::getWidth() const -> std::uint16_t
{
	return m_width;
}

auto Grid::getHeight() const -> std::uint16_t
{
	return m_height;
}

auto Grid::getCellCount() const -> std::size_t
{
	return m_cellCount;
}

auto Grid::getSignedCellCount() const -> std::ptrdiff_t
{
	return m_signedCellCount;
}

auto Grid::getCells() const -> const std::vector< Cell >&
{
	return m_cells;
}

auto Grid::getSpatialGrid() const -> const SpatialGrid&
{
	return m_spatialGrid;
}

auto Grid::copyCreationArguments() const -> Args
{
	return m_creationArguments;
}

auto Grid::cells() -> std::vector< Cell >&
{
	return m_cells;
}

auto Grid::addToSpatialGrid( entt::entity entity, std::size_t cellIndex ) -> void
{
	auto& bucket   = m_spatialGrid[ cellIndex ];
	auto& position = m_registry.get< component::Position >( entity );

	position.spatialIndex = bucket.size();
	position.cellIndex    = cellIndex;
	bucket.push_back( entity );
}

auto Grid::removeFromSpatialGrid( entt::entity targetEntity, std::size_t cellIndex ) -> void
{
	auto& bucket      = m_spatialGrid[ cellIndex ];
	auto& targetIndex = m_registry.get< component::Position >( targetEntity ).spatialIndex;

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