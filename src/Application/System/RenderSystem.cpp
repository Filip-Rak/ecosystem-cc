#include "Application/System/RenderSystem.hpp"

#include <cassert>
#include <vector>

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

#include "Application/Constants/CellConstants.hpp"
#include "Application/Constants/VisModeConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Engine/Utility/Color.hpp"

namespace cc::app
{
namespace
{
auto initGridHandle( entt::registry& /*registry*/, IRenderService& renderer ) -> GridHandle
{
	// TODO: Base visual grid on data from registry.
	// WARN: Same as App.
	constexpr uint16_t FixedDim = 100;
	return renderer.createGrid( FixedDim, FixedDim, glm::vec2{ 0.f }, 4.f );
}

template < auto CellPropertyPtr >
auto colorizeCells( std::vector< Color >& colors, const Grid& grid, float propertyRange,
                    const constant::VisModeConstants& visMode ) -> void
{
	const auto& cells = grid.cells;
	for ( std::size_t index = 0; index < cells.size(); index++ )
	{
		const Cell& cell = cells[ index ];
		Color& color = colors[ index ];

		const float multiplier = cell.*CellPropertyPtr / propertyRange;
		color = lerpColor( visMode.LowEndColor, visMode.HighEndColor, multiplier );
	}
}
}  // namespace
RenderSystem::RenderSystem( entt::registry& registry, IRenderService& renderer )
    : m_renderer( renderer ),
      m_gridHandle( initGridHandle( registry, renderer ) ),
      m_cameraHandle( renderer.createCamera() )
{
	assert( registry.ctx().contains< Camera >() && "Camera not initialized" );
	assert( registry.ctx().contains< Grid >() && "Grid not initialized" );
	assert( registry.ctx().contains< VisualGrid >() && "VisualGrid not initialized" );
}

auto RenderSystem::update( entt::registry& registry ) -> void
{
	const auto& grid = registry.ctx().get< Grid >();
	auto& visualGrid = registry.ctx().get< VisualGrid >();

	updateGridHandle( grid, visualGrid );

	const auto& camera = registry.ctx().get< Camera >();
	updateCameraHandle( camera );
}

auto RenderSystem::updateGridHandle( const Grid& grid, VisualGrid& visualGrid ) -> void
{
	auto& colors = visualGrid.colors;

	using enum VisModeEnum;
	switch ( visualGrid.visMode )
	{
	case Vegetation:
		colorizeCells< &Cell::vegetation >( colors, grid, constant::Cell.VegetationRange,
		                                    constant::VisModes.Vegetation );
		break;
	case Elevation:
		colorizeCells< &Cell::Elevation >( colors, grid, constant::Cell.ElevationRange,
		                                   constant::VisModes.Elevation );
		break;
	case Humidity:
		colorizeCells< &Cell::Humidity >( colors, grid, constant::Cell.HumidityRange,
		                                  constant::VisModes.Humidity );
		break;
	case Temperature:
		colorizeCells< &Cell::Temperature >( colors, grid, constant::Cell.TemperatureRange,
		                                     constant::VisModes.Temperature );
		break;

	default: assert( false && "Unhandled VisMode selected" );
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