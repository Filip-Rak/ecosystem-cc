#include "Application/System/UISystem.hpp"

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/UI/Config.hpp"
#include "Application/UI/MenuBar.hpp"
#include "Application/UI/SidePanel.hpp"
#include "Application/UI/StatusBar.hpp"
#include "Engine/Service/GUIService.hpp"

namespace cc::app
{
UISystem::UISystem( entt::registry& registry )
{
	assert( registry.ctx().contains< GUIService >() && "GUIService uninitialized" );
	registry.ctx().emplace< ui::config::SidePanel >();

	auto& gui = registry.ctx().get< GUIService >();
	gui.addToDraw(
	    []( entt::registry& registry ) -> void
	    {
		    ui::drawMenuBar();
		    ui::drawSidePanel( registry );
		    ui::drawStatusBar();
	    } );
}

auto UISystem::update( entt::registry& registry ) -> void {}
}  // namespace cc::app