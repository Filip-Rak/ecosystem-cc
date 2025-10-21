#include "GUIService.hpp"

#include <imgui.h>

namespace cc
{
GUIService::GUIService()
{
	ImGui::GetIO().IniFilename = nullptr;
}

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