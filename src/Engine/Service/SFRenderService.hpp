#pragma once

#include <cstddef>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <entt/fwd.hpp>

#include "Engine/Interface/IRenderService.hpp"

namespace cc
{
namespace event
{
struct GUIResized;
struct WindowResized;
}  // namespace event

struct Color;

class SFRenderService : public IRenderService
{
public:
	SFRenderService( entt::registry& registry, sf::RenderWindow& window );

	auto beginFrame() -> void override;
	auto endFrame() -> void override;

	[[nodiscard]] auto createCamera() -> CameraHandle override;
	[[nodiscard]] auto getMousePosRelativeToCamera( CameraHandle handle ) const -> glm::vec2 override;
	auto setPosition( CameraHandle handle, glm::vec2 position ) -> void override;
	auto setActiveCamera( CameraHandle handle ) -> void override;
	auto setZoom( CameraHandle handle, float level ) -> void override;

	[[nodiscard]] auto createGrid( std::size_t width, std::size_t height, glm::vec2 position, float cellSize )
	    -> GridHandle override;
	[[nodiscard]] auto getGridCellUnderMouse( GridHandle gridHandle, CameraHandle cameraHandle ) const
	    -> std::optional< glm::vec2 > override;
	auto setGridColors( GridHandle handle, const std::vector< Color >& colors ) -> void override;
	auto draw( GridHandle handle ) -> void override;

private:
	struct GridData
	{
		static const std::size_t VertsPerCell = 6;
		const std::size_t cellNumber;
		const std::size_t rows;
		const std::size_t columns;
		const float cellSize;
		sf::Vector2f position;
		sf::VertexArray vertices;
	};

	struct CameraData
	{
		sf::View view;
		const sf::Vector2f BaseSize;
		float zoomLevel = 1.f;
	};

	auto onGUIResized( const event::GUIResized& event ) -> void;
	auto onWindowResized( const event::WindowResized& event ) -> void;

	entt::registry& m_registry;
	std::vector< GridData > m_gridDataVector;
	std::vector< CameraData > m_cameraVector;
	sf::RenderWindow& m_window;
	bool m_updatedFont = true;
};
}  // namespace cc