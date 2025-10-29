#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Engine/Interface/IRenderService.hpp"

namespace cc
{
namespace event
{
struct RebuildFont;
}

struct Color;

class SFRenderService : public IRenderService
{
public:
	SFRenderService( entt::registry& registry, sf::RenderWindow& window );

	auto beginFrame( entt::registry& registry ) -> void override;
	auto endFrame( entt::registry& registry ) -> void override;

	auto createCamera() -> CameraHandle override;
	auto setPosition( CameraHandle handle, glm::vec2 position ) -> void override;
	auto setCamera( CameraHandle handle ) -> void override;
	auto setZoom( CameraHandle handle, float level ) -> void override;

	auto createGrid( std::size_t width, std::size_t height, glm::vec2 position, float cellSize )
	    -> GridHandle override;
	auto draw( GridHandle handle ) -> void override;

private:
	struct GridData
	{
		float cellSize;
		sf::VertexArray vertices;
	};

	struct CameraData
	{
		sf::View view;
		const sf::Vector2f baseSize;
		float zoomLevel = 1.f;
	};

	auto onRebuildFont( const event::RebuildFont& event ) -> void;

	std::vector< GridData > m_gridDataVector;
	std::vector< CameraData > m_cameraVector;
	sf::RenderWindow& m_window;
	bool m_updatedFont = true;
};
}  // namespace cc