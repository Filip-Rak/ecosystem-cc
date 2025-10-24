#include "Application/UI/StatusBar.hpp"

#include <imgui.h>

namespace cc::app::ui
{
namespace
{
auto setProperties() -> void
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(
	    ImVec2( viewport->WorkPos.x, viewport->WorkPos.y + viewport->WorkSize.y - 25 ) );
	ImGui::SetNextWindowSize( ImVec2( viewport->WorkSize.x, 25 ) );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 10, 5 ) );
}

auto createWidgets() -> void
{
	ImGui::Text( "Running" );
	ImGui::SameLine( ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize( "FPS: 60" ).x );
	ImGui::Text( "FPS: 60" );
}

auto createBar() -> void
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
	                         ImGuiWindowFlags_NoSavedSettings |
	                         ImGuiWindowFlags_NoBringToFrontOnFocus;

	if ( ImGui::Begin( "StatusBar", nullptr, flags ) )
	{
		createWidgets();
		ImGui::End();
	}

	ImGui::PopStyleVar();
}
}  // namespace
auto drawStatusBar() -> void
{
	setProperties();
	createBar();
}
}  // namespace cc::app::ui