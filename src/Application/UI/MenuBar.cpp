#include "Application/UI/MenuBar.hpp"

#include <iostream>  // TODO: Debug

#include <imgui.h>

namespace cc::app::ui
{
namespace
{
auto drawFileMenu() -> void
{
	if ( ImGui::BeginMenu( "File" ) )
	{
		if ( ImGui::MenuItem( "Exit" ) )
		{
			std::cout << "Exit\n";
		}
		ImGui::EndMenu();
	}
}
auto drawEditMenu() -> void
{
	if ( ImGui::BeginMenu( "Edit" ) )
	{
		if ( ImGui::MenuItem( "Speed up" ) )
		{
			std::cout << "Speed up\n";
		}
		else if ( ImGui::MenuItem( "Slow down" ) )
		{
			std::cout << "Slow down\n";
		}
		if ( ImGui::MenuItem( "Pause / Resume" ) )
		{
			std::cout << "Pause / Resume\n";
		}
		ImGui::EndMenu();
	}
}
auto drawHelpMenu() -> void
{
	if ( ImGui::BeginMenu( "Help" ) )
	{
		if ( ImGui::MenuItem( "About" ) )
		{
			std::cout << "About\n";
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
}  // namespace cc::app::ui