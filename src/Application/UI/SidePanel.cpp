#include "Application/UI/SidePanel.hpp"

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/UI/Config.hpp"
#include "Application/UI/Constants.h"

namespace cc::app::ui
{
namespace
{
constexpr const auto& Constants = constant::SidePanel;
constexpr const auto& Labels = Constants.WidgetLabels;

auto setProperties( config::SidePanel& panelConfig )
{
	const auto* const ViewPort = ImGui::GetMainViewport();
	const auto& panelWidth = panelConfig.width;

	constexpr float StatusBarHeight = constant::StatusBar.Height;
	const float PanelTop = ViewPort->WorkPos.y;
	const float PanelLeft = ViewPort->WorkPos.x + ViewPort->WorkSize.x - panelWidth;
	const float PanelHeight = ViewPort->WorkSize.y - StatusBarHeight;

	ImGui::SetNextWindowPos( ImVec2{ PanelLeft, PanelTop }, ImGuiCond_Always );
	ImGui::SetNextWindowSize( ImVec2{ panelWidth, PanelHeight }, ImGuiCond_Always );
}

auto drawWidgets() -> void
{
	ImGui::Text( "Dummy" );
	static float dummyVal = 1.f;
	ImGui::SliderFloat( "Dummy", &dummyVal, 0.0f, 1.0f );
}

auto drawPanel( config::SidePanel& panelConfig ) -> void
{
	if ( ImGui::Begin( Labels.SidePanel.data(), nullptr, Constants.MainWindowFlags ) )
	{
		panelConfig.width = ImGui::GetWindowSize().x;

		constexpr const auto& ScrollPanel = Constants.ScrollablePanel;
		if ( ImGui::BeginChild( Labels.ScrollablePanel.data(), ScrollPanel.Size,
		                        ScrollPanel.ChildFlags, ScrollPanel.WindowFlags ) )
		{
			drawWidgets();
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

}  // namespace

auto drawSidePanel( entt::registry& registry ) -> void
{
	auto& panelConfig = registry.ctx().get< config::SidePanel >();
	setProperties( panelConfig );
	drawPanel( panelConfig );
}
}  // namespace cc::app::ui