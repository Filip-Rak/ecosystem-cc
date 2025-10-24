#include "Application/UI/StatusBar.hpp"

#include <imgui.h>

namespace cc::app::ui
{
namespace
{
struct Bar
{
	float sizeY = 25.f;
	ImVec2 padding{ 10.f, 5.f };
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
	                         ImGuiWindowFlags_NoSavedSettings |
	                         ImGuiWindowFlags_NoBringToFrontOnFocus;
} constexpr bar;

auto setProperties() -> void
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	const float uiScale = ImGui::GetIO().FontGlobalScale;
	const float height = bar.sizeY * uiScale;
	const ImVec2 padding{ bar.padding.x * uiScale, bar.padding.y * uiScale };

	// clang-format off
	ImGui::SetNextWindowPos( ImVec2( viewport->WorkPos.x, viewport->WorkPos.y + viewport->WorkSize.y - height ) );
	ImGui::SetNextWindowSize( ImVec2( viewport->WorkSize.x, height ) );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, padding );
	// clang-format on
}

auto createWidgets() -> void
{
	// TODO: Implement.
	// clang-format off
    ImGui::Text("Running");
    ImGui::SameLine(); ImGui::TextDisabled("|");
    ImGui::SameLine(); ImGui::Text("Mouse: (%d, %d)", 34, 24);
    ImGui::SameLine(); ImGui::Text("|");
    ImGui::SameLine(); ImGui::Text("Zoom: %.1f×", 1.f);
    ImGui::SameLine(); ImGui::Text("|");
    ImGui::SameLine(); ImGui::Text("Seed: %u", 720472);
    ImGui::SameLine(); ImGui::Text("|");
    ImGui::SameLine(); ImGui::Text("IN File: %s", "input.json");
    ImGui::SameLine(); ImGui::Text("|");
    ImGui::SameLine(); ImGui::Text("OUT File: %s", "output.csv");

    ImGui::SameLine(); ImGui::Text("|");
    ImGui::SameLine(); ImGui::Text("FPS: %.0f", 60.f);
	// clang-format on
}

auto createBar() -> void
{
	if ( ImGui::Begin( "StatusBar", nullptr, bar.flags ) )
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