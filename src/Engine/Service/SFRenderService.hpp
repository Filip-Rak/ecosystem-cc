#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Engine/Interface/IRenderService.hpp"
#include "Engine/Utility/RenderHandles.hpp"  // TODO: Should this header be exposed to others? In interface even?

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

	auto createGrid( std::size_t width, std::size_t height, glm::vec2 position, float cellSize )
	    -> GridHandle override;
	auto draw( GridHandle handle ) -> void override;

private:
	struct GridData
	{
		float cellSize;
		sf::VertexArray vertices;
	};

	auto onRebuildFont( const event::RebuildFont& event ) -> void;

	std::vector< GridData > m_gridDataVector;
	sf::RenderWindow& m_window;
	bool m_updatedFont = true;
};
}  // namespace cc