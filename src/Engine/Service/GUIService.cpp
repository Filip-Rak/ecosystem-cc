#include "GUIService.hpp"

namespace cc
{
void GUIService::addToDraw( DrawFunction drawFunction )
{
	m_drawFunctions.push_back( std::move( drawFunction ) );
}

void GUIService::endFrame( entt::registry& registry )
{
	for ( auto& drawFunction : m_drawFunctions )
	{
		drawFunction( registry );
	}
}
}  // namespace cc