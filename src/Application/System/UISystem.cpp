#include "Application/System/UISystem.hpp"

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/ContextEntity/UIConfig.hpp"
#include "Application/UI/MenuBar.hpp"
#include "Application/UI/SidePanel.hpp"
#include "Application/UI/StatusBar.hpp"
#include "Engine/Service/GUIService.hpp"

namespace cc::app
{
UISystem::UISystem( entt::registry& registry )
{
	assert( registry.ctx().contains< GUIService >() && "GUIService uninitialized" );
	assert( registry.ctx().contains< UIConfig >() && "UIConfig uninitialized" );

	auto& gui = registry.ctx().get< GUIService >();
	gui.addToDraw(
	    []( entt::registry& registry ) -> void
	    {
		    drawMenuBar( registry );
		    drawSidePanel( registry );
		    drawStatusBar( registry );
	    } );
}  // namespace

auto UISystem::update( entt::registry& registry ) -> void {}
}  // namespace cc::app