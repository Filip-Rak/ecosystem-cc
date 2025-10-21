#include "Application/System/UISystem.hpp"

#include <entt/entt.hpp>
#include <imgui.h>

#include "Engine/Service/GUIService.hpp"

namespace cc::app
{
UISystem::UISystem( entt::registry& registry )
{
	assert( registry.ctx().contains< GUIService >() && "GUIService uninitialized" );

	auto& gui = registry.ctx().get< GUIService >();
	gui.addToDraw(
	    []( entt::registry& /*registry*/ ) -> void
	    {
		    ImGui::Begin( "ImGui Test" );

		    static int click_count = 0;

		    ImGui::Text( "ImGui!" );
		    ImGui::Separator();

		    if ( ImGui::Button( "Click me" ) )
		    {
			    click_count++;
		    }

		    ImGui::SameLine();
		    ImGui::Text( "Clicked %d times", click_count );

		    ImGui::End();
	    } );
}

auto UISystem::update( entt::registry& registry ) -> void {}
}  // namespace cc::app