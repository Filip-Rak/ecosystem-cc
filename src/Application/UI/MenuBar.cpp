#include "Application/UI/MenuBar.hpp"

#include <iostream>  // TODO: Debug

#include <imgui.h>

#include "Application/Constants/UIConstants.hpp"

namespace cc::app
{
namespace
{
constexpr const auto& Labels = constant::MenuBar.WidgetLabels;

auto drawFileMenu() -> void
{
	if ( ImGui::BeginMenu( Labels.File.data() ) )
	{
		if ( ImGui::MenuItem( Labels.Exit.data() ) )
		{
			std::cout << Labels.Exit.data() << "\n";
		}
		ImGui::EndMenu();
	}
}

auto drawEditMenu() -> void
{
	if ( ImGui::BeginMenu( Labels.Edit.data() ) )
	{
		if ( ImGui::MenuItem( Labels.SpeedUp.data() ) )
		{
			std::cout << Labels.SpeedUp.data() << "\n";
		}
		if ( ImGui::MenuItem( Labels.SlowDown.data() ) )
		{
			std::cout << Labels.SlowDown.data() << "\n";
		}
		if ( ImGui::MenuItem( Labels.Resume.data() ) )
		{
			std::cout << Labels.Resume.data() << " -> " << Labels.Pause.data() << "\n";
		}
		ImGui::EndMenu();
	}
}

auto drawHelpMenu() -> void
{
	if ( ImGui::BeginMenu( Labels.Help.data() ) )
	{
		if ( ImGui::MenuItem( Labels.About.data() ) )
		{
			std::cout << Labels.About.data() << "\n";
		}
		ImGui::EndMenu();
	}
}
}  // namespace
auto drawMenuBar() -> void
{
	if ( ImGui::BeginMainMenuBar() )
	{
		drawFileMenu();
		drawEditMenu();
		drawHelpMenu();
		ImGui::EndMainMenuBar();
	}
}
}  // namespace cc::app