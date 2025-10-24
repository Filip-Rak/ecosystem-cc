#pragma once

#include <string_view>

#include <imgui.h>

namespace cc::app::ui::constant
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

} inline constexpr MenuBar;

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
		std::string_view Zoom = "Zoom: %.1fx";
		std::string_view Seed = "Seed: %u";
		std::string_view INFile = "IN File: %s";
		std::string_view OUTFile = "OUT File: %s";
		std::string_view FPS = "FPS: %.0f";
	} Labels;
} constexpr StatusBar;
}  // namespace cc::app::ui::constant
