#include "Application/UI/MenuBar.hpp"

#include <print>  // TODO: Debug

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/Constants/UIConstants.hpp"
#include "Engine/Events/SystemEvents.hpp"

namespace cc::app
{
namespace
{
constexpr const auto& Labels = constant::UI.MenuBar.WidgetLabels;

auto drawFileMenu( entt::registry& registry ) -> void
{
	if ( ImGui::BeginMenu( Labels.File.data() ) )
	{
		if ( ImGui::MenuItem( Labels.Exit.data() ) )
		{
			auto& dispatcher = registry.ctx().get< entt::dispatcher >();
			dispatcher.enqueue< event::Exit >();
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
			std::println( "{}", Labels.SpeedUp.data() );
		}
		if ( ImGui::MenuItem( Labels.SlowDown.data() ) )
		{
			std::println( "{}", Labels.SlowDown.data() );
		}
		if ( ImGui::MenuItem( Labels.Resume.data() ) )
		{
			std::println( "{} -> {}", Labels.Resume.data(), Labels.Pause.data() );
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
			std::println( "{}", Labels.About.data() );
		}
		ImGui::EndMenu();
	}
}
}  // namespace
auto drawMenuBar( entt::registry& registry ) -> void
{
	if ( ImGui::BeginMainMenuBar() )
	{
		drawFileMenu( registry );
		drawEditMenu();
		drawHelpMenu();
		ImGui::EndMainMenuBar();
	}
}
}  // namespace cc::app