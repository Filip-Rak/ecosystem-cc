#include "Application/System/RenderSystem.hpp"

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

#include "Application/Constants/CellConstants.hpp"
#include "Application/Constants/VisualConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Grid.hpp"
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
	const auto& logicalGrid = registry.ctx().get< Grid >();

	const uint16_t width  = logicalGrid.getWidth();
	const uint16_t height = logicalGrid.getHeight();

	constexpr float CellSize   = 4.f;
	const float verticalSize   = static_cast< float >( width ) * CellSize;
	const float horizontalSize = static_cast< float >( height ) * CellSize;
	const glm::vec2 position{ -( verticalSize / 2.f ), -( horizontalSize / 2.f ) };

	return renderer.createGrid( width, height, position, CellSize );
}

template < auto CellPropertyPtr >
auto colorizeCells( std::vector< Color >& colors, const Grid& grid, float propertyRange,
                    const constant::Visual_t::VisMode_t& visMode ) -> void
{
	const auto& cells = grid.getCells();
	for ( std::size_t index = 0; index < cells.size(); index++ )
	{
		const Cell& cell = cells[ index ];
		Color& color     = colors[ index ];

		const float factor = std::min( cell.*CellPropertyPtr / propertyRange, 1.f );
		color              = lerpColor( visMode.LowEndColor, visMode.HighEndColor, factor );
	}
}

auto colorizePopulationCells( std::vector< Color >& colors,
                              const std::vector< std::vector< entt::entity > >& spatialGrid ) -> void
{
	// TODO: Consider performance.
	const auto highestPopulation = std::ranges::max( spatialGrid | std::views::transform( std::ranges::size ) );
	// const auto highestPopulation = 2uz;

	constexpr const auto& Constants = constant::Visual.VisModes.Population;
	for ( std::size_t index = 0; index < spatialGrid.size(); index++ )
	{
		const std::size_t cellPopulation = spatialGrid[ index ].size();
		Color& color                     = colors[ index ];

		const float popFactor =
		    std::min( static_cast< float >( cellPopulation ) / static_cast< float >( highestPopulation ), 1.f );
		color = lerpColor( Constants.LowEndColor, Constants.HighEndColor, popFactor );
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

auto RenderSystem::updateGridHandle( const Grid& grid, VisualGrid& visualGrid ) -> void
{
	constexpr const auto& VisMode = constant::Visual.VisModes;
	constexpr const auto& Cell    = constant::cell;
	auto& colors                  = visualGrid.colors;

	using enum VisModeEnum;
	switch ( visualGrid.visMode )
	{
	case Vegetation:
	{
		colorizeCells< &Cell::vegetation >( colors, grid, Cell.vegetationRange, VisMode.Vegetation );
		break;
	}
	case Elevation:
	{
		colorizeCells< &Cell::elevation >( colors, grid, Cell.elevationRange, VisMode.Elevation );
		break;
	}
	case Humidity:
	{
		colorizeCells< &Cell::humidity >( colors, grid, Cell.humidityRange, VisMode.Humidity );
		break;
	}
	case Temperature:
	{
		colorizeCells< &Cell::temperature >( colors, grid, Cell.temperatureRange, VisMode.Temperature );
		break;
	}
	case Population:
	{
		colorizePopulationCells( colors, grid.getSpatialGrid() );
		break;
	}
	default: assert( false );
	}

	m_renderer.setGridColors( m_gridHandle, colors );
}

auto RenderSystem::updateCameraHandle( const Camera& camera ) -> void
{
	m_renderer.setPosition( m_cameraHandle, camera.position );
	m_renderer.setZoom( m_cameraHandle, camera.zoomLevel );

	m_renderer.setActiveCamera( m_cameraHandle );
	m_renderer.draw( m_gridHandle );
}
}  // namespace cc::app