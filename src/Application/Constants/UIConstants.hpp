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
			std::string_view File = "File";
			std::string_view Exit = "Exit";
			std::string_view Edit = "Edit";
			std::string_view SpeedUp = "Speed up";
			std::string_view SlowDown = "Slow down";
			std::string_view Resume = "Resume";
			std::string_view Pause = "Pause";
			std::string_view Help = "Help";
			std::string_view About = "About";
		} WidgetLabels;

	} const MenuBar;

	struct SidePanel_t
	{
		float InitialWidth = 300.f;
		ImGuiWindowFlags MainWindowFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		                                   ImGuiWindowFlags_NoBringToFrontOnFocus |
		                                   ImGuiWindowFlags_NoSavedSettings;

		struct Contents_t
		{
			int minSpeed = 1;
			int maxSpeed = 240;
			float maxUIScale = 5.f;
			float minUIScale = 0.5f;
			std::string_view sliderPrecision = "%.2f";
		} Contents;

		struct WidgetLabels_t
		{
			std::string_view SidePanel = "Side Panel";
			std::string_view ScrollablePanel = "Scrollable Panel";
			std::string_view ZoomSlider = "Zoom";
			std::string_view SpeedSlider = "Speed";
			std::string_view PauseButtonPaused = "Resume";
			std::string_view PauseButtonRunning = "Pause";
			std::string_view UIScaleSlider = "UI Scale";
			std::string_view IncrementButton = "Increment";
			std::string_view RestartButton = "Restart";
			std::string_view VisModeCombo = "Legend";
		} WidgetLabels;

		struct ScrollablePanel_t
		{
			ImGuiWindowFlags ChildFlags = ImGuiChildFlags_None;
			ImGuiWindowFlags WindowFlags =
			    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
			ImVec2 Size = ImVec2( -1, -1 );
		} ScrollablePanel;
	} const SidePanel;

	struct StatusBar_t
	{
		float Height = 25.f;
		ImVec2 Padding{ 10.f, 5.f };
		ImGuiWindowFlags Flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
		                         ImGuiWindowFlags_NoSavedSettings |
		                         ImGuiWindowFlags_NoBringToFrontOnFocus;

		struct Labels_t
		{
			std::string_view Separator = "|";
			std::string_view Running = "Running";
			std::string_view Mouse = "Mouse: (%d, %d)";
			std::string_view Seed = "Seed: %u";
			std::string_view INFile = "IN File: %s";
			std::string_view OUTFile = "OUT File: %s";
			std::string_view FPS = "FPS: %.0f";
			std::string_view RunTime = "Run time: %.0fs";
		} Labels;
	} const StatusBar;
} constexpr UI;
}  // namespace cc::app::constant
