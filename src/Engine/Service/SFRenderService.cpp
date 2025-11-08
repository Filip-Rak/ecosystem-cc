#include "Engine/Service/SFRenderService.hpp"

#include <print>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <entt/entt.hpp>
#include <glm/vec2.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "Engine/Events/GUIEvents.hpp"
#include "Engine/Events/WindowEvents.hpp"
#include "Engine/Service/GUIService.hpp"
#include "Engine/Utility/SFMath.hpp"

namespace cc
{
namespace
{
auto rebuildFont( entt::registry& registry ) -> void
{
	auto& gui = registry.ctx().get< GUIService >();
	gui.rebuildFont();

	if ( !ImGui::SFML::UpdateFontTexture() )
	{
		std::println( stderr, "SFRenderService::rebuildFont() -> Font update failed" );
	}
}

auto letterboxViewport( float windowWidth, float windowHeight, float viewWidth, float viewHeight ) -> sf::FloatRect
{
	const float windowRatio = windowWidth / windowHeight;
	const float viewRatio = viewWidth / viewHeight;

	float sizeX = 1.f;
	float sizeY = 1.f;
	float posX = 0.f;
	float posY = 0.f;

	constexpr float half = 0.5f;

	if ( windowRatio > viewRatio )
	{
		sizeX = viewRatio / windowRatio;
		posX = ( 1.f - sizeX ) * half;
	}
	else
	{
		sizeY = windowRatio / viewRatio;
		posY = ( 1.f - sizeY ) * half;
	}

	return { { posX, posY }, { sizeX, sizeY } };
}
}  // namespace

SFRenderService::SFRenderService( entt::registry& registry, sf::RenderWindow& window ) : m_window( window )
{
	assert( registry.ctx().contains< entt::dispatcher >() && "Dispatcher not initialized" );
	assert( registry.ctx().contains< GUIService >() && "GUIService not initialized" );

	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::GUIResized >().connect< &SFRenderService::onGUIResized >( *this );
	dispatcher.sink< event::WindowResized >().connect< &SFRenderService::onWindowResized >( *this );
}

auto SFRenderService::beginFrame( entt::registry& /*registry*/ ) -> void
{
	m_window.clear();
}

auto SFRenderService::endFrame( entt::registry& registry ) -> void
{
	ImGui::SFML::Render( m_window );
	m_window.display();

	if ( !m_updatedFont )
	{
		rebuildFont( registry );
		m_updatedFont = true;
	}
}

auto SFRenderService::createCamera() -> CameraHandle
{
	const auto& view = m_window.getView();
	auto& camera = m_cameraVector.emplace_back( view, view.getSize() );

	return { static_cast< uint16_t >( m_cameraVector.size() - 1 ) };
}

auto SFRenderService::setPosition( CameraHandle handle, glm::vec2 position ) -> void
{
	auto& view = m_cameraVector[ handle.index ].view;
	view.setCenter( toSf( position ) );
}

auto SFRenderService::setActiveCamera( CameraHandle handle ) -> void
{
	m_window.setView( m_cameraVector[ handle.index ].view );
}

auto SFRenderService::setZoom( CameraHandle handle, float level ) -> void
{
	auto& camera = m_cameraVector[ handle.index ];

	const sf::Vector2f newSize = camera.BaseSize / level;
	camera.view.setSize( newSize );
}

auto SFRenderService::createGrid( std::size_t width, std::size_t height, glm::vec2 position, float cellSize ) -> GridHandle
{
	const std::size_t cellNumber = width * height;

	auto& gridData = m_gridDataVector.emplace_back( cellNumber );
	auto& vertices = gridData.vertices;

	vertices.setPrimitiveType( sf::PrimitiveType::Triangles );
	vertices.resize( cellNumber * GridData::VertsPerCell );
	for ( std::size_t index = 0; index < cellNumber; index++ )
	{
		const auto yInt = index / width;
		const auto y = static_cast< float >( yInt );
		const auto x = static_cast< float >( index % width );

		const float left = x * cellSize;
		const float top = y * cellSize;
		const float right = left + cellSize;
		const float bottom = top + cellSize;

		const sf::Vector2f a{ left, top };
		const sf::Vector2f b{ right, top };
		const sf::Vector2f c{ right, bottom };
		const sf::Vector2f d{ left, bottom };

		const std::size_t vertexBase = index * GridData::VertsPerCell;

		const std::size_t A0 = vertexBase + 0;
		const std::size_t B0 = vertexBase + 1;
		const std::size_t C0 = vertexBase + 2;
		const std::size_t A1 = vertexBase + 3;
		const std::size_t C1 = vertexBase + 4;
		const std::size_t D1 = vertexBase + 5;

		vertices[ A0 ].position = position + a;
		vertices[ B0 ].position = position + b;
		vertices[ C0 ].position = position + c;

		vertices[ A1 ].position = position + a;
		vertices[ C1 ].position = position + c;
		vertices[ D1 ].position = position + d;
	}

	return { static_cast< uint16_t >( m_gridDataVector.size() - 1 ) };
}

auto SFRenderService::setGridColors( GridHandle& handle, const std::vector< Color >& colors ) -> void
{
	auto& grid = m_gridDataVector[ handle.index ];
	assert( grid.cellNumber == colors.size() && "Grid and color size mismatch" );

	for ( std::size_t cellIndex = 0; cellIndex < colors.size(); cellIndex++ )
	{
		const std::size_t vertexBase = cellIndex * GridData::VertsPerCell;
		for ( std::size_t vertIndex = 0; vertIndex < GridData::VertsPerCell; vertIndex++ )
		{
			auto& sfColor = grid.vertices[ vertexBase + vertIndex ].color;
			const auto& glmColorToApply = colors[ cellIndex ];

			sfColor.r = glmColorToApply.red;
			sfColor.g = glmColorToApply.green;
			sfColor.b = glmColorToApply.blue;
			sfColor.a = glmColorToApply.alpha;
		}
	}
}

auto SFRenderService::draw( GridHandle handle ) -> void
{
	const auto& grid = m_gridDataVector[ handle.index ];
	m_window.draw( grid.vertices );
}

auto SFRenderService::onGUIResized( const event::GUIResized& /*event*/ ) -> void
{
	m_updatedFont = false;
}

auto SFRenderService::onWindowResized( const event::WindowResized& event ) -> void
{
	for ( auto& cam : m_cameraVector )
	{
		const sf::Vector2f size = cam.view.getSize();
		cam.view.setViewport( letterboxViewport( event.width, event.height, size.x, size.y ) );
	}
}
}  // namespace cc