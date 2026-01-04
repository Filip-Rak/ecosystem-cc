#include "Application/UI/SidePanel.hpp"

#include <print>  // TODO: Debug

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/Constants/UIConstants.hpp"
#include "Application/Constants/VisualConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/GuiLog.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/ContextEntity/UIConfig.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Application/Events/SimRunnerEvents.hpp"
#include "Engine/Events/GUIEvents.hpp"

namespace cc::app
{
namespace
{
constexpr const auto& constants = constant::UI.SidePanel;
constexpr const auto& labels    = constants.WidgetLabels;

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

auto drawGuiLog( const GuiLog& log ) -> void
{
	ImGui::NewLine();
	ImGui::Text( "Total Births: %zu", log.totalBirths );
	ImGui::Text( "Highest population: %zu", log.highestPopulation );
	ImGui::Text( "Current Population: %zu", log.currentPopulation );
	ImGui::Text( "Starvations: %zu", log.totalStarvations );
	ImGui::Text( "Old Age Deaths: %zu", log.oldAgeDeaths );
	ImGui::Text( "Eaten vegetation: %f", log.totalVegetationEaten );
	ImGui::Text( "Average max energy: %f", log.averageEnergyGene );
	ImGui::Text( "Average adaptation: %f", log.averageAdaptation );
	ImGui::Text( "Average food preference: %f", log.averageFoodPref );
	ImGui::Text( "Average RP: %f", log.averageRefractoryPeriod );
}

auto drawContents( entt::registry& registry ) -> void
{
	constexpr const auto& contents = constants.contents;
	constexpr const auto& Visual   = constant::visual;
	auto& panelConfig              = registry.ctx().get< UIConfig >().sidePanel;
	auto& cam                      = registry.ctx().get< Camera >();

	auto& simRunnerData = registry.ctx().get< SimRunnerData >();
	auto& preset        = registry.ctx().get< Preset >();

	ImGui::LabelText( labels.IterationLabel.data(), "%zu of %zu", simRunnerData.iteration, preset.iterationTarget );

	ImGui::SliderFloat( labels.ZoomSlider.data(), &cam.zoomLevel, Visual.MinZoom, Visual.MaxZoom,
	                    contents.sliderPrecision.data(), ImGuiSliderFlags_AlwaysClamp );
	ImGui::SliderInt( labels.SpeedSlider.data(), &simRunnerData.speed, contents.minSpeed, contents.maxSpeed, "%d",
	                  ImGuiSliderFlags_AlwaysClamp );

	auto& config = registry.ctx().get< UIConfig >();
	const auto pauseButtonAltLabel =
	    ( simRunnerData.targetReached ) ? labels.PauseButtonFinished : labels.PauseButtonPaused;
	const auto pauseButtonLabel = ( simRunnerData.paused ) ? pauseButtonAltLabel : labels.PauseButtonRunning;

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
	if ( ImGui::Button( labels.IncrementButton.data() ) )
	{
		std::println( "{}", labels.IncrementButton.data() );
	}

	ImGui::SameLine();

	ImGui::BeginDisabled( simRunnerData.iteration == 0 );
	if ( ImGui::Button( labels.RestartButton.data() ) )
	{
		auto& dispatcher = registry.ctx().get< entt::dispatcher >();
		dispatcher.enqueue< event::ResetSim >();
		simRunnerData.paused = true;

		std::println( "{}", labels.RestartButton.data() );
	}
	ImGui::EndDisabled();

	float& uiScale = ImGui::GetIO().FontGlobalScale;
	if ( ImGui::SliderFloat( labels.UIScaleSlider.data(), &uiScale, contents.minUIScale, contents.maxUIScale,
	                         contents.sliderPrecision.data(), ImGuiSliderFlags_AlwaysClamp ) )
	{
		auto& dispatcher = registry.ctx().get< entt::dispatcher >();
		dispatcher.enqueue< cc::event::GUIResized >();
	}

	VisModeEnum& currentVisMode       = registry.ctx().get< VisualGrid >().visMode;
	auto currentVisModeInt            = static_cast< int >( currentVisMode );
	constexpr const auto& visModesArr = app::constant::visual.visModes.array;

	if ( ImGui::BeginCombo( labels.VisModeCombo.data(), app::constant::getVisModeData( currentVisMode ).name.data() ) )
	{
		for ( int index = 0; const auto& mode : visModesArr )
		{
			bool isSelected = ( currentVisModeInt == index );
			if ( ImGui::Selectable( mode.name.data(), isSelected ) )
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
		const auto visModeData = visModesArr[ currentVisModeInt ];

		const ImU32 leftColor  = toImGui( visModeData.lowEndColor );
		const ImU32 rightColor = toImGui( visModeData.highEndColor );

		const float boxWidth = ImGui::GetContentRegionAvail().x * constants.ScrollablePanel.DefaultWidgetWidthFactor;
		const ImVec2 boxSize( boxWidth, contents.GradientBoxHeight * uiScale );
		const ImVec2 cursorPos = ImGui::GetCursorScreenPos();

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilledMultiColor( cursorPos, ImVec2( cursorPos.x + boxSize.x, cursorPos.y + boxSize.y ),
		                                   leftColor, rightColor, rightColor, leftColor );
		ImGui::Dummy( boxSize );

		const auto* const leftLabel  = visModeData.lowEndName.data();
		const auto* const rightLabel = visModeData.highEndName.data();

		ImGui::TextUnformatted( leftLabel );

		const float leftTextWidth   = ImGui::CalcTextSize( leftLabel ).x;
		const float rightTextWidth  = ImGui::CalcTextSize( rightLabel ).x;
		const float rightTextBeginX = boxWidth - rightTextWidth;

		const float minimumDistX = contents.LegendLabelsMinDistX * uiScale;
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
		const auto& guiLog = registry.ctx().get< GuiLog >();
		drawGuiLog( guiLog );
	}
}

auto drawPanel( entt::registry& registry ) -> void
{
	auto& panelConfig = registry.ctx().get< UIConfig >().sidePanel;
	if ( ImGui::Begin( labels.SidePanel.data(), nullptr, constants.MainWindowFlags ) )
	{
		panelConfig.width = ImGui::GetWindowSize().x;

		constexpr const auto& ScrollPanel = constants.ScrollablePanel;
		if ( ImGui::BeginChild( labels.ScrollablePanel.data(), ScrollPanel.Size, ScrollPanel.ChildFlags,
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