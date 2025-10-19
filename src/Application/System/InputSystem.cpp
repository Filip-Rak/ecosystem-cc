#include "Application/System/InputSystem.hpp"

#include <iostream>  // FIXME: Debug

#include <entt/entt.hpp>

#include "Engine/Events/SystemEvents.hpp"
#include "Engine/Service/InputService.hpp"
#include "Engine/Utility/Time.hpp"

namespace cc::eco
{
auto InputSystem::update( entt::registry& registry ) -> void
{
	// FIXME: Debug
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