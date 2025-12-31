#pragma once

#include <string_view>

#include <imgui.h>

namespace cc::app::constant
{
struct UI_t
{
	struct MenuBar_t
	{
		struct WidgetLabels_t
		{
			const std::string_view File     = "File";
			const std::string_view Exit     = "Exit";
			const std::string_view Edit     = "Edit";
			const std::string_view SpeedUp  = "Speed up";
			const std::string_view SlowDown = "Slow down";
			const std::string_view Resume   = "Resume";
			const std::string_view Pause    = "Pause";
			const std::string_view Help     = "Help";
			const std::string_view About    = "About";
		} const WidgetLabels;

	} const MenuBar;

	struct SidePanel_t
	{
		const float InitialWidth               = 300.f;
		const ImGuiWindowFlags MainWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		                                         ImGuiWindowFlags_NoBringToFrontOnFocus |
		                                         ImGuiWindowFlags_NoSavedSettings;

		struct Contents_t
		{
			const int minSpeed                     = 1;
			const int maxSpeed                     = 720;
			const float maxUIScale                 = 5.f;
			const float minUIScale                 = 0.5f;
			const float GradientBoxHeight          = 40.f;
			const float LegendLabelsMinDistX       = 10.f;
			const std::string_view sliderPrecision = "%.2f";
		} const contents;

		struct WidgetLabels_t
		{
			const std::string_view SidePanel           = "Side Panel";
			const std::string_view IterationLabel      = "Iteration: ";
			const std::string_view ScrollablePanel     = "Scrollable Panel";
			const std::string_view ZoomSlider          = "Zoom";
			const std::string_view SpeedSlider         = "Speed";
			const std::string_view PauseButtonPaused   = "Resume";
			const std::string_view PauseButtonRunning  = "Pause";
			const std::string_view PauseButtonFinished = "Continue";
			const std::string_view UIScaleSlider       = "UI Scale";
			const std::string_view IncrementButton     = "Increment";
			const std::string_view RestartButton       = "Restart";
			const std::string_view VisModeCombo        = "Legend";
		} const WidgetLabels;

		struct ScrollablePanel_t
		{
			const ImGuiWindowFlags ChildFlags    = ImGuiChildFlags_None;
			const ImGuiWindowFlags WindowFlags   = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
			const ImVec2 Size                    = ImVec2( -1, -1 );
			const float DefaultWidgetWidthFactor = 0.65;
		} const ScrollablePanel;
	} const SidePanel;

	struct StatusBar_t
	{
		const float Height = 25.f;
		const ImVec2 Padding{ 10.f, 5.f };
		const ImGuiWindowFlags Flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
		                               ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;

		struct Labels_t
		{
			const std::string_view Separator      = "|";
			const std::string_view StatusRunning  = "Running";
			const std::string_view StatusPaused   = "Paused";
			const std::string_view StatusFinished = "Finished";
			const std::string_view HoveredCell    = "Hovered Cell:";
			const std::string_view Seed           = "Seed: %u";
			const std::string_view Preset         = "Preset: %s";
			const std::string_view OUTFile        = "OUT File: %s";
			const std::string_view FPS            = "FPS: %.0f";
			const std::string_view RunTime        = "Run time: %.0fs";
		} const Labels;
	} const StatusBar;
} inline constexpr UI;
}  // namespace cc::app::constant
