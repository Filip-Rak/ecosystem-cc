#include "Application/UI/StatusBar.hpp"

#include <string>

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/Constants/UIConstants.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Engine/ContextEntity/Time.hpp"

namespace cc::app
{
namespace
{
constexpr const auto& Constants = constant::UI.StatusBar;
constexpr const auto& Separator = Constants.Labels.Separator;
constexpr const auto& Labels = Constants.Labels;

auto setProperties() -> void
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	const float UiScale = ImGui::GetIO().FontGlobalScale;
	const float ScaledHeight = Constants.Height * UiScale;
	const ImVec2 ScaledPadding{ Constants.Padding.x * UiScale, Constants.Padding.y * UiScale };

	const ImVec2 NextWindowSize{ viewport->WorkSize.x, ScaledHeight };
	const ImVec2 NextWindowPos{ viewport->WorkPos.x, viewport->WorkPos.y + viewport->WorkSize.y - ScaledHeight };

	ImGui::SetNextWindowSize( NextWindowSize );
	ImGui::SetNextWindowPos( NextWindowPos );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ScaledPadding );
}

auto createWidgets( entt::registry& registry ) -> void
{
	const auto& runnerData = registry.ctx().get< SimRunnerData >();
	const auto& preset = registry.ctx().get< Preset >();

	// clang-format off
	const auto& StoppedAltStatusLabel = ( runnerData.targetReached ) ? Labels.StatusFinished : Labels.StatusPaused;
    ImGui::Text( "%s", (runnerData.paused) ?  StoppedAltStatusLabel.data() : Labels.StatusRunning.data()   );
    ImGui::SameLine(); ImGui::Text( Separator.data() );

	const auto& visualGrid = registry.ctx().get<VisualGrid>();
	if (const auto mousePos = visualGrid.cellPositionUnderMouse; mousePos)
	{
		std::string MouseLabel = Labels.HoveredCell.data() + std::string(" ( %d, %d )");
		ImGui::SameLine(); ImGui::Text( MouseLabel.c_str(), mousePos->x, mousePos->y );
	}
	else 
	{
		std::string MouseLabel = Labels.HoveredCell.data() + std::string(" ( Outside grid )");
		ImGui::SameLine(); ImGui::Text( "%s", MouseLabel.c_str() );
	}

    ImGui::SameLine(); ImGui::Text( Separator.data() );

	
    ImGui::SameLine(); ImGui::Text( Labels.Preset.data(), preset.presetName.c_str());
    ImGui::SameLine(); ImGui::Text( Separator.data() );
	
    ImGui::SameLine(); ImGui::Text( Labels.Seed.data(), preset.rngSeed);
    ImGui::SameLine(); ImGui::Text( Separator.data() );
	
	// TODO: Implement.
    ImGui::SameLine(); ImGui::Text( Labels.OUTFile.data(), "output.csv");
    ImGui::SameLine(); ImGui::Text( Separator.data() );
	
	const auto& time = registry.ctx().get< Time >();
    ImGui::SameLine(); ImGui::Text( Labels.RunTime.data(), time.RunTime );
	ImGui::SameLine(); ImGui::Text( Separator.data() );

    ImGui::SameLine(); ImGui::Text( Labels.FPS.data(), time.FPS );
	// clang-format on
}

auto createBar( entt::registry& registry ) -> void
{
	if ( ImGui::Begin( "StatusBar", nullptr, Constants.Flags ) )
	{
		createWidgets( registry );
		ImGui::End();
	}

	ImGui::PopStyleVar();
}
}  // namespace
auto drawStatusBar( entt::registry& registry ) -> void
{
	setProperties();
	createBar( registry );
}
}  // namespace cc::app