#include "Application/UI/MenuBar.hpp"

#include <algorithm>
#include <print>  // TODO: Debug
#include <string>

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/Constants/UIConstants.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Engine/Events/SystemEvents.hpp"

namespace cc::app
{
namespace
{
constexpr const auto& labels = constant::UI.MenuBar.WidgetLabels;

auto drawFileMenu( entt::registry& registry ) -> void
{
	if ( ImGui::BeginMenu( labels.File.data() ) )
	{
		if ( ImGui::MenuItem( labels.Exit.data() ) )
		{
			auto& dispatcher = registry.ctx().get< entt::dispatcher >();
			dispatcher.enqueue< event::Exit >();
		}
		ImGui::EndMenu();
	}
}

auto drawEditMenu( entt::registry& registry ) -> void
{
	auto& runnerData              = registry.ctx().get< SimRunnerData >();
	constexpr const auto contents = constant::UI.SidePanel.contents;
	constexpr const auto maxSpeed = contents.maxSpeed;
	constexpr const auto minSpeed = contents.minSpeed;
	constexpr auto speedIncrement = 60;

	if ( ImGui::BeginMenu( labels.Edit.data() ) )
	{
		if ( ImGui::MenuItem( labels.SpeedUp.data() ) )
		{
			runnerData.speed += speedIncrement;
			runnerData.speed = std::clamp( runnerData.speed, minSpeed, maxSpeed );
			std::println( "{}", labels.SpeedUp.data() );
		}
		if ( ImGui::MenuItem( labels.SlowDown.data() ) )
		{
			runnerData.speed -= speedIncrement;
			runnerData.speed = std::clamp( runnerData.speed, minSpeed, maxSpeed );
			std::println( "{}", labels.SlowDown.data() );
		}

		const std::string playString = ( runnerData.paused ) ? labels.Resume.data() : labels.Pause.data();
		if ( ImGui::MenuItem( playString.c_str() ) )
		{
			runnerData.paused = !runnerData.paused;
			std::println( "{} -> {}", labels.Resume.data(), labels.Pause.data() );
		}
		ImGui::EndMenu();
	}
}

auto drawHelpMenu() -> void
{
	if ( ImGui::BeginMenu( labels.Help.data() ) )
	{
		if ( ImGui::MenuItem( labels.About.data() ) )
		{
			std::println( "{}", labels.About.data() );
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
		drawEditMenu( registry );
		drawHelpMenu();
		ImGui::EndMainMenuBar();
	}
}
}  // namespace cc::app