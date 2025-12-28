#include "Application/UI/SidePanel.hpp"

#include <print>  // TODO: Debug

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/Constants/UIConstants.hpp"
#include "Application/Constants/VisualConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/SimLog.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/ContextEntity/UIConfig.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Application/Events/SimRunnerEvents.hpp"
#include "Engine/Events/GUIEvents.hpp"

namespace cc::app
{
namespace
{
constexpr const auto& Constants = constant::UI.SidePanel;
constexpr const auto& Labels    = Constants.WidgetLabels;

auto toImGui( Color color ) -> ImU32
{
	return IM_COL32( color.red, color.green, color.blue, color.alpha );
}

auto setProperties( entt::registry& registry )
{
	const auto& panelConfig    = registry.ctx().get< UIConfig >().sidePanel;
	const auto& panelWidth     = panelConfig.width;
	const auto* const ViewPort = ImGui::GetMainViewport();

	constexpr float StatusBarHeight = constant::UI.StatusBar.Height;
	const float PanelTop            = ViewPort->WorkPos.y;
	const float PanelLeft           = ViewPort->WorkPos.x + ViewPort->WorkSize.x - panelWidth;
	const float UiScale             = ImGui::GetIO().FontGlobalScale;
	const float PanelHeight         = ViewPort->WorkSize.y - ( StatusBarHeight * UiScale );

	ImGui::SetNextWindowPos( ImVec2{ PanelLeft, PanelTop }, ImGuiCond_Always );
	ImGui::SetNextWindowSize( ImVec2{ panelWidth, PanelHeight }, ImGuiCond_Always );
}

auto drawSimLog( const SimLog& log ) -> void
{
	ImGui::NewLine();
	ImGui::Text( "Total Births: %zu", log.totalBirths );
	ImGui::Text( "Current Population: %zu", log.currentPopulation );
	ImGui::Text( "Starvations: %zu", log.starvations );
	ImGui::Text( "Old Age Deaths: %zu", log.oldAgeDeaths );
}

auto drawContents( entt::registry& registry ) -> void
{
	constexpr const auto& Contents = Constants.Contents;
	constexpr const auto& Visual   = constant::Visual;
	auto& panelConfig              = registry.ctx().get< UIConfig >().sidePanel;
	auto& cam                      = registry.ctx().get< Camera >();

	auto& simRunnerData = registry.ctx().get< SimRunnerData >();
	auto& preset        = registry.ctx().get< Preset >();

	ImGui::LabelText( Labels.IterationLabel.data(), "%zu of %zu", simRunnerData.iteration, preset.iterationTarget );

	ImGui::SliderFloat( Labels.ZoomSlider.data(), &cam.zoomLevel, Visual.MinZoom, Visual.MaxZoom,
	                    Contents.sliderPrecision.data(), ImGuiSliderFlags_AlwaysClamp );
	ImGui::SliderInt( Labels.SpeedSlider.data(), &simRunnerData.speed, Contents.minSpeed, Contents.maxSpeed, "%d",
	                  ImGuiSliderFlags_AlwaysClamp );

	auto& config = registry.ctx().get< UIConfig >();
	const auto pauseButtonAltLabel =
	    ( simRunnerData.targetReached ) ? Labels.PauseButtonFinished : Labels.PauseButtonPaused;
	const auto pauseButtonLabel = ( simRunnerData.paused ) ? pauseButtonAltLabel : Labels.PauseButtonRunning;

	if ( ImGui::Button( pauseButtonLabel.data() ) )
	{
		if ( config.askBeforeContinuing )
		{
			config.askBeforeContinuing = false;
			std::println( "Continuing past target" );
		}

		simRunnerData.paused = !simRunnerData.paused;
	}

	ImGui::SameLine();
	if ( ImGui::Button( Labels.IncrementButton.data() ) )
	{
		std::println( "{}", Labels.IncrementButton.data() );
	}

	ImGui::SameLine();

	ImGui::BeginDisabled( simRunnerData.iteration == 0 );
	if ( ImGui::Button( Labels.RestartButton.data() ) )
	{
		auto& dispatcher = registry.ctx().get< entt::dispatcher >();
		dispatcher.enqueue< event::ResetSim >();
		simRunnerData.paused = true;

		std::println( "{}", Labels.RestartButton.data() );
	}
	ImGui::EndDisabled();

	float& uiScale = ImGui::GetIO().FontGlobalScale;
	if ( ImGui::SliderFloat( Labels.UIScaleSlider.data(), &uiScale, Contents.minUIScale, Contents.maxUIScale,
	                         Contents.sliderPrecision.data(), ImGuiSliderFlags_AlwaysClamp ) )
	{
		auto& dispatcher = registry.ctx().get< entt::dispatcher >();
		dispatcher.enqueue< cc::event::GUIResized >();
	}

	VisModeEnum& currentVisMode       = registry.ctx().get< VisualGrid >().visMode;
	auto currentVisModeInt            = static_cast< int >( currentVisMode );
	constexpr const auto& VisModesArr = app::constant::Visual.VisModes.Array;

	if ( ImGui::BeginCombo( Labels.VisModeCombo.data(), app::constant::getVisModeData( currentVisMode ).Name.data() ) )
	{
		for ( int index = 0; const auto& mode : VisModesArr )
		{
			bool isSelected = ( currentVisModeInt == index );
			if ( ImGui::Selectable( mode.Name.data(), isSelected ) )
			{
				currentVisModeInt = index;
				currentVisMode    = static_cast< VisModeEnum >( currentVisModeInt );
			}
			if ( isSelected ) ImGui::SetItemDefaultFocus();

			++index;
		}
		ImGui::EndCombo();
	}

	{
		const auto visModeData = VisModesArr[ currentVisModeInt ];

		const ImU32 leftColor  = toImGui( visModeData.LowEndColor );
		const ImU32 rightColor = toImGui( visModeData.HighEndColor );

		const float boxWidth = ImGui::GetContentRegionAvail().x * Constants.ScrollablePanel.DefaultWidgetWidthFactor;
		const ImVec2 boxSize( boxWidth, Contents.GradientBoxHeight * uiScale );
		const ImVec2 cursorPos = ImGui::GetCursorScreenPos();

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilledMultiColor( cursorPos, ImVec2( cursorPos.x + boxSize.x, cursorPos.y + boxSize.y ),
		                                   leftColor, rightColor, rightColor, leftColor );
		ImGui::Dummy( boxSize );

		const auto* const leftLabel  = visModeData.LowEndName.data();
		const auto* const rightLabel = visModeData.HighEndName.data();

		ImGui::TextUnformatted( leftLabel );

		const float leftTextWidth   = ImGui::CalcTextSize( leftLabel ).x;
		const float rightTextWidth  = ImGui::CalcTextSize( rightLabel ).x;
		const float rightTextBeginX = boxWidth - rightTextWidth;

		const float minimumDistX = Contents.LegendLabelsMinDistX * uiScale;
		if ( boxWidth >= rightTextWidth + leftTextWidth + minimumDistX )
		{
			ImGui::SameLine();
		}
		if ( rightTextBeginX > 0 )
		{
			ImGui::SetCursorPosX( rightTextBeginX );
		}

		ImGui::TextUnformatted( rightLabel );
	}

	{
		const auto& simLog = registry.ctx().get< SimLog >();
		drawSimLog( simLog );
	}
}

auto drawPanel( entt::registry& registry ) -> void
{
	auto& panelConfig = registry.ctx().get< UIConfig >().sidePanel;
	if ( ImGui::Begin( Labels.SidePanel.data(), nullptr, Constants.MainWindowFlags ) )
	{
		panelConfig.width = ImGui::GetWindowSize().x;

		constexpr const auto& ScrollPanel = Constants.ScrollablePanel;
		if ( ImGui::BeginChild( Labels.ScrollablePanel.data(), ScrollPanel.Size, ScrollPanel.ChildFlags,
		                        ScrollPanel.WindowFlags ) )
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
}  // namespace cc::app