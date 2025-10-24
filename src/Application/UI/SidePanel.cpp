#include "Application/UI/SidePanel.hpp"

#include <imgui.h>

namespace cc::app::ui
{
auto drawSidePanel() -> void
{
	const auto* const ViewPort = ImGui::GetMainViewport();

	static float panelWidth = 300.f;

	constexpr float StatusBarHeight = 25.f;  // TODO: Replace with actual
	const float PanelTop = ViewPort->WorkPos.y;
	const float PanelLeft = ViewPort->WorkPos.x + ViewPort->WorkSize.x - panelWidth;
	const float PanelHeight = ViewPort->WorkSize.y - StatusBarHeight;

	ImGui::SetNextWindowPos( ImVec2{ PanelLeft, PanelTop }, ImGuiCond_Always );
	ImGui::SetNextWindowSize( ImVec2{ panelWidth, PanelHeight }, ImGuiCond_Always );

	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
	                                   ImGuiWindowFlags_NoBringToFrontOnFocus |
	                                   ImGuiWindowFlags_NoSavedSettings;

	if ( ImGui::Begin( "SidePanel", nullptr, flags ) )
	{
		panelWidth = ImGui::GetWindowSize().x;

		ImGui::TextUnformatted( "Inspector" );
		ImGui::Separator();

		if ( ImGui::BeginChild( "RightPanelContent", ImVec2( -1, -1 ), ImGuiChildFlags_None,
		                        ImGuiWindowFlags_NoMove ) )
		{
			ImGui::Text( "Property A" );
			static float opacity = 1.f;
			ImGui::SliderFloat( "Opacity", &opacity, 0.0f, 1.0f );
		}
		ImGui::EndChild();
	}
	ImGui::End();
}
}  // namespace cc::app::ui