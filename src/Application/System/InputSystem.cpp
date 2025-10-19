#include "Application/System/InputSystem.hpp"

#include <iostream>  // FIXME: Debug

#include <entt/entt.hpp>

#include "Engine/Events/SystemEvents.hpp"
#include "Engine/Service/InputService.hpp"
#include "Engine/Utility/Time.hpp"

namespace cc::eco
{
InputSystem::InputSystem( entt::registry& registry )
{
	// TODO: Add the same check in services's init method
	assert( registry.ctx().contains< InputService >() && "InputService not initialized" );
	assert( registry.ctx().contains< Time >() && "Time not initialized" );
}

auto InputSystem::update( entt::registry& registry ) -> void
{
	// TODO: Debug
	const auto& input = registry.ctx().get< InputService >();
	const auto& time = registry.ctx().get< Time >();

	if ( input.isPressed( keyboard::Key::Space ) )
	{
		std::cout << "Run time: " << time.RunTime << "\n";
		std::cout << "FPS: " << time.FPS << "\n";
		std::cout << "DeltaTime: " << time.DeltaTime << "\n";
	}

	if ( input.isPressed( keyboard::Key::Escape ) )
	{
		auto& dispatcher = registry.ctx().get< entt::dispatcher >();
		dispatcher.enqueue< event::Exit >();
	}
}
}  // namespace cc::eco