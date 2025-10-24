#include "Application/UI/StatusBar.hpp"

#include <imgui.h>

#include "Application/UI/Constants.h"

namespace cc::app::ui
{
namespace
{
constexpr const auto& Constants = constant::StatusBar;
constexpr const auto& Labels = Constants.Labels;

auto setProperties() -> void
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	const float UiScale = ImGui::GetIO().FontGlobalScale;
	const float ScaledHeight = Constants.Height * UiScale;
	const ImVec2 ScaledPadding{ Constants.Padding.x * UiScale, Constants.Padding.y * UiScale };

	const ImVec2 NextWindowSize{ viewport->WorkSize.x, ScaledHeight };
	const ImVec2 NextWindowPos{ viewport->WorkPos.x,
	                            viewport->WorkPos.y + viewport->WorkSize.y - ScaledHeight };

	ImGui::SetNextWindowSize( NextWindowSize );
	ImGui::SetNextWindowPos( NextWindowPos );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ScaledPadding );
}

auto createWidgets() -> void
{
	// TODO: Implement.
	// clang-format off
    ImGui::Text( Labels.Running.data() );
    ImGui::SameLine(); ImGui::Text( Labels.Separator.data() );
    ImGui::SameLine(); ImGui::Text( Labels.Mouse.data(), 34, 24 );
    ImGui::SameLine(); ImGui::Text( Labels.Separator.data() );
    ImGui::SameLine(); ImGui::Text( Labels.Zoom.data(), 1.f );
    ImGui::SameLine(); ImGui::Text( Labels.Separator.data() );
    ImGui::SameLine(); ImGui::Text( Labels.Seed.data(), 720472);
    ImGui::SameLine(); ImGui::Text( Labels.Separator.data() );
    ImGui::SameLine(); ImGui::Text( Labels.INFile.data(), "input.json");
    ImGui::SameLine(); ImGui::Text( Labels.Separator.data() );
    ImGui::SameLine(); ImGui::Text( Labels.OUTFile.data(), "output.csv");

    ImGui::SameLine(); ImGui::Text( Labels.Separator.data() );
    ImGui::SameLine(); ImGui::Text( Labels.FPS.data(), 60.f);
	// clang-format on
}

auto createBar() -> void
{
	if ( ImGui::Begin( "StatusBar", nullptr, Constants.Flags ) )
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