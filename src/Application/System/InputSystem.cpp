#include "Application/System/InputSystem.hpp"

#include <cassert>
#include <iostream>  // FIXME: Debug

#include <entt/entt.hpp>

#include "Application/ContextEntity/Camera.hpp"
#include "Engine/Events/SystemEvents.hpp"
#include "Engine/Service/InputService.hpp"
#include "Engine/Utility/Time.hpp"

namespace cc::app
{
namespace
{
// TODO: Debug
auto updateDebug( entt::registry& registry, const InputService& input, const Time& time ) -> void
{
	if ( input.isPressed( keyboard::Key::Space ) )
	{
		std::cout << "Run time: " << time.RunTime << "\n";
		std::cout << "FPS: " << time.FPS << "\n";
		std::cout << "DeltaTime: " << time.DeltaTime << "\n";
	}

	if ( input.isPressed( keyboard::Key::Escape ) )
	{
		auto& dispatcher = registry.ctx().get< entt::dispatcher >();
		dispatcher.enqueue< event::Exit >();  // FIXME: Should this be a trigger, should .update be
		                                      // in the loop?
	}
}
}  // namespace
InputSystem::InputSystem( entt::registry& registry )
{
	assert( registry.ctx().contains< InputService >() && "InputService not initialized" );
	assert( registry.ctx().contains< Time >() && "Time not initialized" );
	assert( registry.ctx().contains< Camera >() && "Camera not initialized" );
}

auto InputSystem::update( entt::registry& registry ) -> void
{
	const auto& input = registry.ctx().get< InputService >();
	const auto& time = registry.ctx().get< Time >();

	auto& camera = registry.ctx().get< Camera >();

	camera.movementInput = { 0.f, 0.f };
	if ( input.isDown( keyboard::Key::W ) ) camera.movementInput.y -= 1;
	if ( input.isDown( keyboard::Key::S ) ) camera.movementInput.y += 1;
	if ( input.isDown( keyboard::Key::A ) ) camera.movementInput.x -= 1;
	if ( input.isDown( keyboard::Key::D ) ) camera.movementInput.x += 1;

	camera.zoomInput = 0.f;
	if ( input.isDown( keyboard::Key::Up ) ) camera.zoomInput -= 1.f;
	if ( input.isDown( keyboard::Key::Down ) ) camera.zoomInput += 1.f;

	updateDebug( registry, input, time );
}
}  // namespace cc::app