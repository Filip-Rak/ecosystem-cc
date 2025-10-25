#pragma once

#include <string_view>

#include <imgui.h>

namespace cc::app::ui::constant
{
struct MenuBar_t
{
	struct WidgetLabels_t
	{
		static constexpr std::string_view File = "File";
		static constexpr std::string_view Exit = "Exit";
		static constexpr std::string_view Edit = "Edit";
		static constexpr std::string_view SpeedUp = "Speed up";
		static constexpr std::string_view SlowDown = "Slow down";
		static constexpr std::string_view Resume = "Resume";
		static constexpr std::string_view Pause = "Pause";
		static constexpr std::string_view Help = "Help";
		static constexpr std::string_view About = "About";
	} WidgetLabels;

} constexpr MenuBar;

struct StatusBar_t
{
	static constexpr float Height = 25.f;
	static constexpr ImVec2 Padding{ 10.f, 5.f };
	static constexpr ImGuiWindowFlags Flags =
	    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
	    ImGuiWindowFlags_NoBringToFrontOnFocus;

	struct Labels_t
	{
		static constexpr std::string_view Separator = "|";
		static constexpr std::string_view Running = "Running";
		static constexpr std::string_view Mouse = "Mouse: (%d, %d)";
		static constexpr std::string_view Zoom = "Zoom: %.1fx";
		static constexpr std::string_view Seed = "Seed: %u";
		static constexpr std::string_view INFile = "IN File: %s";
		static constexpr std::string_view OUTFile = "OUT File: %s";
		static constexpr std::string_view FPS = "FPS: %.0f";
	} Labels;
} constexpr StatusBar;

struct SidePanel_t
{
	static constexpr float InitialWidth = 300.f;
	static constexpr ImGuiWindowFlags MainWindowFlags =
	    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
	    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings;

	struct ScrollablePanel_t
	{
		static constexpr ImGuiWindowFlags ChildFlags = ImGuiChildFlags_None;
		static constexpr ImGuiWindowFlags WindowFlags =
		    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
		static constexpr ImVec2 Size = ImVec2( -1, -1 );
	} ScrollablePanel;

	struct WidgetLabels_t
	{
		static constexpr std::string_view SidePanel = "Side Panel";
		static constexpr std::string_view ScrollablePanel = "Scrollable Panel";
	} WidgetLabels;
} constexpr SidePanel;
}  // namespace cc::app::ui::constant
