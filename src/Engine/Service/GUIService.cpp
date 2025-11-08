#include "GUIService.hpp"

#include <imgui.h>

namespace cc
{
GUIService::GUIService() : m_IO( ImGui::GetIO() )
{
	m_IO.IniFilename = nullptr;
}

void GUIService::endFrame( entt::registry& registry )
{
	for ( auto& drawFunction : m_drawFunctions )
	{
		drawFunction( registry );
	}
}

void GUIService::addToDraw( DrawFunction drawFunction )
{
	m_drawFunctions.push_back( std::move( drawFunction ) );
}

auto GUIService::nowHandlesMouse() const -> bool
{
	return m_IO.WantCaptureMouse;
}
}  // namespace cc