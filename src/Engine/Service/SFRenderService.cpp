#include "Engine/Service/SFRenderService.hpp"

#include <iostream>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <entt/entt.hpp>
#include <glm/vec2.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "Engine/Events/GUIEvents.hpp"
#include "Engine/Utility/SFMath.hpp"

namespace cc
{
namespace
{
// TODO: Figure out a better place for this -> should be part of the base.
auto rebuildFont() -> void
{
	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->Clear();

	const float UiScale = io.FontGlobalScale;
	constexpr float BaseFontSize = 13.f;

	ImFontConfig cfg;
	// cfg.SizePixels = BaseFontSize * UiScale;  // FIXME: Do something about this stupidity.

	io.Fonts->AddFontDefault( &cfg );
	io.Fonts->Build();

	if ( !ImGui::SFML::UpdateFontTexture() )
	{
		std::cerr << "SFRenderService::rebuildFont() -> Font update failed\n";
	}
}
}  // namespace

SFRenderService::SFRenderService( entt::registry& registry, sf::RenderWindow& window )
    : m_window( window )
{
	assert( registry.ctx().contains< entt::dispatcher >() && "Dispatcher not initialized" );
	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::RebuildFont >().connect< &SFRenderService::onRebuildFont >( *this );
}

auto SFRenderService::beginFrame( entt::registry& /*registry*/ ) -> void
{
	m_window.clear();
}

auto SFRenderService::endFrame( entt::registry& /*registry*/ ) -> void
{
	ImGui::SFML::Render( m_window );
	m_window.display();

	if ( !m_updatedFont )
	{
		rebuildFont();
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

auto SFRenderService::setCamera( CameraHandle handle ) -> void
{
	m_window.setView( m_cameraVector[ handle.index ].view );
}

auto SFRenderService::setZoom( CameraHandle handle, float level ) -> void
{
	auto& camera = m_cameraVector[ handle.index ];

	const sf::Vector2f newSize = camera.baseSize * level;
	camera.view.setSize( newSize );
}

auto SFRenderService::createGrid( std::size_t width, std::size_t height, glm::vec2 position,
                                  float cellSize ) -> GridHandle
{
	constexpr std::size_t VertsPerCell = 6;
	const std::size_t cellNumber = width * height;

	sf::VertexArray vertices( sf::PrimitiveType::Triangles, cellNumber * VertsPerCell );
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

		const std::size_t vertexBase = index * VertsPerCell;

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

	m_gridDataVector.emplace_back( cellSize, std::move( vertices ) );
	return { static_cast< uint16_t >( m_gridDataVector.size() - 1 ) };
}

auto SFRenderService::draw( GridHandle handle ) -> void
{
	const auto& grid = m_gridDataVector[ handle.index ];
	m_window.draw( grid.vertices );
}

auto SFRenderService::onRebuildFont( const event::RebuildFont& /*event*/ ) -> void
{
	m_updatedFont = false;
}
}  // namespace cc