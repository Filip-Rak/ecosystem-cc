#include "Application/System/UISystem.hpp"

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/ContextEntity/UIConfig.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Application/UI/MenuBar.hpp"
#include "Application/UI/SidePanel.hpp"
#include "Application/UI/StatusBar.hpp"
#include "Engine/ContextEntity/Time.hpp"
#include "Engine/Service/GUIService.hpp"

namespace cc::app
{
UISystem::UISystem( entt::registry& registry )
{
	assert( registry.ctx().contains< entt::dispatcher >() && "Dispatcher uninitialized" );
	assert( registry.ctx().contains< SimRunnerData >() && "SimRunnerData uninitialized" );
	assert( registry.ctx().contains< VisualGrid >() && "VisualGrid uninitialized" );
	assert( registry.ctx().contains< GUIService >() && "GUIService uninitialized" );
	assert( registry.ctx().contains< UIConfig >() && "UIConfig uninitialized" );
	assert( registry.ctx().contains< Camera >() && "Camera uninitialized" );
	assert( registry.ctx().contains< Time >() && "Time uninitialized" );

	auto& gui = registry.ctx().get< GUIService >();
	gui.addToDraw(
	    []( entt::registry& registry ) -> void
	    {
		    drawMenuBar( registry );
		    drawSidePanel( registry );
		    drawStatusBar( registry );
	    } );
}  // namespace

auto UISystem::update() -> void {}
}  // namespace cc::app