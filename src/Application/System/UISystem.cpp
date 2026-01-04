#include "Application/System/UISystem.hpp"

#include <entt/entt.hpp>
#include <imgui.h>

#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/ContextEntity/TickDataCollection.hpp"
#include "Application/ContextEntity/UIConfig.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Application/Events/SimRunnerEvents.hpp"
#include "Application/UI/MenuBar.hpp"
#include "Application/UI/SidePanel.hpp"
#include "Application/UI/StatusBar.hpp"
#include "Engine/ContextEntity/Time.hpp"
#include "Engine/Service/GUIService.hpp"

namespace cc::app
{
UISystem::UISystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< entt::dispatcher >() );
	assert( registry.ctx().contains< SimRunnerData >() );
	assert( registry.ctx().contains< VisualGrid >() );
	assert( registry.ctx().contains< GUIService >() );
	assert( registry.ctx().contains< UIConfig >() );
	assert( registry.ctx().contains< TickDataCollection >() );
	assert( registry.ctx().contains< Camera >() );
	assert( registry.ctx().contains< Preset >() );
	assert( registry.ctx().contains< Time >() );

	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::ReachedTargetIteration >().connect< &UISystem::onReachedTargetIteration >( *this );

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

auto UISystem::onReachedTargetIteration( const event::ReachedTargetIteration& /*event*/ ) -> void
{
	auto& config               = m_registry.ctx().get< UIConfig >();
	config.askBeforeContinuing = true;
}
}  // namespace cc::app