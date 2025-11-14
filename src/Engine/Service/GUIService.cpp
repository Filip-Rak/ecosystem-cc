#include "GUIService.hpp"

#include <imgui.h>

namespace cc
{
GUIService::GUIService( entt::registry& registry ) : m_registry( registry ), m_IO( ImGui::GetIO() )
{
	m_IO.IniFilename = nullptr;
}

void GUIService::endFrame()
{
	for ( auto& drawFunction : m_drawFunctions )
	{
		drawFunction( m_registry );
	}
}

void GUIService::addToDraw( DrawFunction drawFunction )
{
	m_drawFunctions.push_back( std::move( drawFunction ) );
}

auto GUIService::rebuildFont() const -> void
{
	m_IO.Fonts->Clear();

	// TODO: Use font config instead.
	// const float UiScale = m_IO.FontGlobalScale;
	// constexpr float BaseFontSize = 13.f;

	ImFontConfig cfg;
	// cfg.SizePixels = BaseFontSize * UiScale;

	m_IO.Fonts->AddFontDefault( &cfg );
	m_IO.Fonts->Build();
}

auto GUIService::nowHandlesMouse() const -> bool
{
	return m_IO.WantCaptureMouse;
}
}  // namespace cc