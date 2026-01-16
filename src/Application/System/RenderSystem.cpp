#include "Application/System/RenderSystem.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <ranges>
#include <vector>

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Constants/CellConstants.hpp"
#include "Application/Constants/VisualConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/VisMode.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Engine/Interface/IRenderService.hpp"
#include "Engine/Utility/Color.hpp"

namespace cc::app
{
namespace
{
auto initGridHandle( entt::registry& registry, IRenderService& renderer ) -> GridHandle
{
	assert( registry.ctx().contains< Grid >() );
	const auto& visualGrid = registry.ctx().get< VisualGrid >();

	const uint16_t width  = visualGrid.width;
	const uint16_t height = visualGrid.height;

	constexpr float cellSize   = 4.f;
	const float verticalSize   = static_cast< float >( width ) * cellSize;
	const float horizontalSize = static_cast< float >( height ) * cellSize;
	const glm::vec2 position{ -( verticalSize / 2.f ), -( horizontalSize / 2.f ) };

	return renderer.createGrid( width, height, position, cellSize );
}

template < auto CellPropertyPtr >
auto colorizeCells( std::vector< Color >& colors, const Grid& grid, float propertyRange,
                    const constant::Visual::VisMode& visMode ) -> void
{
	const auto& cells = grid.cells();
	for ( std::size_t index = 0; index < cells.size(); index++ )
	{
		const Cell& cell = cells[ index ];
		Color& color     = colors[ index ];

		const float factor = std::min( cell.*CellPropertyPtr / propertyRange, 1.f );
		color              = lerpColor( visMode.lowEndColor, visMode.highEndColor, factor );
	}
}

auto colorizePopulationCells( std::vector< Color >& colors,
                              const std::vector< std::vector< entt::entity > >& spatialGrid ) -> void
{
	// TODO: Consider performance.
	const auto highestPopulation = std::ranges::max( spatialGrid | std::views::transform( std::ranges::size ) );

	constexpr const auto& constants = constant::visual.visModes.population;
	for ( std::size_t index = 0; index < spatialGrid.size(); index++ )
	{
		const std::size_t cellPopulation = spatialGrid[ index ].size();
		Color& color                     = colors[ index ];

		const float popFactor =
		    std::min( static_cast< float >( cellPopulation ) / static_cast< float >( highestPopulation ), 1.f );
		color = lerpColor( constants.lowEndColor, constants.highEndColor, popFactor );
	}
}

// FIXME: This is a terrible way of doing it. A lot of bad things added recently I had added only because I am
// running out of time for the thesis. Half the things in app logic could have been done better with couple of extra
// months. Yes, a couple of months - this is app is genuinely big and it would have been twice as big had I not started
// severely cutting corners.
auto colorizeMaxEnergy( entt::registry& registry, std::vector< Color >& colors,
                        const std::vector< std::vector< entt::entity > >& spatialGrid ) -> void
{
	const auto view = registry.view< component::GeneSet >();

	float maxEnergyGene = 0.f;
	float minEnergyGene = 0.f;
	for ( const auto& [ entity, geneSet ] : view.each() )
	{
		const float energyGene = geneSet.agentGenes.maxEnergy;
		maxEnergyGene          = std::max( maxEnergyGene, energyGene );
		minEnergyGene          = std::min( minEnergyGene, energyGene );
	}

	const float range = maxEnergyGene - minEnergyGene;

	for ( auto index{ 0uz }; index < spatialGrid.size(); index++ )
	{
		const auto& spatialCell = spatialGrid[ index ];

		float energySum = 0.f;
		int count       = 0;
		for ( auto entity : spatialCell )
		{
			const auto& agentGenes = registry.get< component::GeneSet >( entity ).agentGenes;
			energySum += agentGenes.maxEnergy;
			count++;
		}

		float average = energySum / static_cast< float >( count );
		float scale   = 0.f;
		if ( count > 0 && range > 0.f )
		{
			const float average = energySum / static_cast< float >( count );

			scale = ( average - minEnergyGene ) / range;
			scale = std::clamp( scale, 0.f, 1.f );
		}

		constexpr const auto& constants = constant::visual.visModes.avgMaxEnergy;

		auto& color = colors[ index ];
		color       = lerpColor( constants.lowEndColor, constants.highEndColor, scale );
	}
}
}  // namespace
RenderSystem::RenderSystem( entt::registry& registry, IRenderService& renderer )
    : m_registry( registry ),
      m_renderer( renderer ),
      m_gridHandle( initGridHandle( registry, renderer ) ),
      m_cameraHandle( renderer.createCamera() )
{
	assert( registry.ctx().contains< Camera >() );
	assert( registry.ctx().contains< VisualGrid >() );
}

auto RenderSystem::update() -> void
{
	const auto& grid = m_registry.ctx().get< Grid >();
	auto& visualGrid = m_registry.ctx().get< VisualGrid >();

	updateGridHandle( grid, visualGrid );
	visualGrid.cellPositionUnderMouse = m_renderer.getGridCellUnderMouse( m_gridHandle, m_cameraHandle );

	const auto& camera = m_registry.ctx().get< Camera >();
	updateCameraHandle( camera );
}

auto RenderSystem::updateGridHandle( const Grid& logicalGrid, VisualGrid& visualGrid ) -> void
{
	constexpr const auto& visMode = constant::visual.visModes;
	constexpr const auto& cell    = constant::cell;
	auto& gridColors              = visualGrid.colors;

	using enum VisModeEnum;
	switch ( visualGrid.visMode )
	{
	case Vegetation:
	{
		colorizeCells< &Cell::vegetation >( gridColors, logicalGrid, cell.vegetationRange, visMode.vegetation );
		break;
	}
	case Flesh:
	{
		colorizeCells< &Cell::flesh >( gridColors, logicalGrid, cell.fleshRange, visMode.flesh );
		break;
	}
	case Population:
	{
		colorizePopulationCells( gridColors, logicalGrid.getSpatialGrid() );
		break;
	}
	case Temperature:
	{
		colorizeCells< &Cell::temperature >( gridColors, logicalGrid, cell.temperatureRange, visMode.temperature );
		break;
	}
	case Humidity:
	{
		colorizeCells< &Cell::humidity >( gridColors, logicalGrid, cell.humidityRange, visMode.humidity );
		break;
	}
	case Elevation:
	{
		colorizeCells< &Cell::elevation >( gridColors, logicalGrid, cell.elevationRange, visMode.elevation );
		break;
	}
	case AvgMaxEnergy:
	{
		colorizeMaxEnergy( m_registry, gridColors, logicalGrid.getSpatialGrid() );
		break;
	}
	default: assert( false );
	}

	m_renderer.setGridColors( m_gridHandle, gridColors );
}

auto RenderSystem::updateCameraHandle( const Camera& camera ) -> void
{
	m_renderer.setPosition( m_cameraHandle, camera.position );
	m_renderer.setZoom( m_cameraHandle, camera.zoomLevel );

	m_renderer.setActiveCamera( m_cameraHandle );
	m_renderer.draw( m_gridHandle );
}
}  // namespace cc::app