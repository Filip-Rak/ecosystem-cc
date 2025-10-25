#include "Application/UI/SidePanel.hpp"

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/UI/Config.hpp"
#include "Application/UI/Constants.h"
#include "Engine/Events/GUIEvents.hpp"

namespace cc::app::ui
{
namespace
{
constexpr const auto& Constants = constant::SidePanel;
constexpr const auto& Labels = Constants.WidgetLabels;

auto setProperties( entt::registry& registry )
{
	auto& panelConfig = registry.ctx().get< config::SidePanel >();
	const auto* const ViewPort = ImGui::GetMainViewport();
	const auto& panelWidth = panelConfig.width;

	constexpr float StatusBarHeight = constant::StatusBar.Height;
	const float PanelTop = ViewPort->WorkPos.y;
	const float PanelLeft = ViewPort->WorkPos.x + ViewPort->WorkSize.x - panelWidth;
	const float UiScale = ImGui::GetIO().FontGlobalScale;
	const float PanelHeight = ViewPort->WorkSize.y - ( StatusBarHeight * UiScale );

	ImGui::SetNextWindowPos( ImVec2{ PanelLeft, PanelTop }, ImGuiCond_Always );
	ImGui::SetNextWindowSize( ImVec2{ panelWidth, PanelHeight }, ImGuiCond_Always );
}

auto drawContents( entt::registry& registry ) -> void
{
	auto& panelConfig = registry.ctx().get< config::SidePanel >();
	constexpr const auto& Contents = Constants.Contents;

	// FIXME: Not for UI config but internal for renderer
	static float zoom = 5.f;
	static int speed = 5.f;

	ImGui::SliderFloat( Labels.ZoomSlider.data(), &zoom, Contents.minZoom, Contents.maxZoom,
	                    Contents.sliderPrecision.data(), ImGuiSliderFlags_AlwaysClamp );
	ImGui::SliderInt( Labels.SpeedSlider.data(), &speed, Contents.minSpeed, Contents.maxSpeed, "%d",
	                  ImGuiSliderFlags_AlwaysClamp );

	if ( ImGui::Button( panelConfig.pauseButtonLabel.c_str() ) )
	{
		// TODO: Ask the sim instead of toggle.
		static bool toggle = true;
		panelConfig.pauseButtonLabel =
		    ( toggle ) ? Labels.PauseButtonRunning : Labels.PauseButtonPaused;
		toggle = !toggle;
	}

	float& uiScale = ImGui::GetIO().FontGlobalScale;
	if ( ImGui::SliderFloat( Labels.UIScaleSlider.data(), &uiScale, Contents.minUIScale,
	                         Contents.maxUIScale, Contents.sliderPrecision.data(),
	                         ImGuiSliderFlags_AlwaysClamp ) )
	{
		auto& dispatcher = registry.ctx().get< entt::dispatcher >();
		dispatcher.enqueue< event::RebuildFont >();
	}
}

auto drawPanel( entt::registry& registry ) -> void
{
	auto& panelConfig = registry.ctx().get< config::SidePanel >();
	if ( ImGui::Begin( Labels.SidePanel.data(), nullptr, Constants.MainWindowFlags ) )
	{
		panelConfig.width = ImGui::GetWindowSize().x;

		constexpr const auto& ScrollPanel = Constants.ScrollablePanel;
		if ( ImGui::BeginChild( Labels.ScrollablePanel.data(), ScrollPanel.Size,
		                        ScrollPanel.ChildFlags, ScrollPanel.WindowFlags ) )
		{
			drawContents( registry );
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

}  // namespace

auto drawSidePanel( entt::registry& registry ) -> void
{
	setProperties( registry );
	drawPanel( registry );
}
}  // namespace cc::app::ui