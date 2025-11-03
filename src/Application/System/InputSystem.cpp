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
		// TODO: Switch to print?
		std::cout << "Run time: " << time.RunTime << "\n";
		std::cout << "FPS: " << time.FPS << "\n";
		std::cout << "DeltaTime: " << time.DeltaTime << "\n";
		std::cout << "Mouse Position { " << input.getCurrentMousePos().x << ", "
		          << input.getCurrentMousePos().y << " }\n";
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

	camera.keyboardMovementInput = { 0.f, 0.f };
	if ( input.isDown( keyboard::Key::W ) ) camera.keyboardMovementInput.y -= 1;
	if ( input.isDown( keyboard::Key::S ) ) camera.keyboardMovementInput.y += 1;
	if ( input.isDown( keyboard::Key::A ) ) camera.keyboardMovementInput.x -= 1;
	if ( input.isDown( keyboard::Key::D ) ) camera.keyboardMovementInput.x += 1;

	if ( input.isDown( mouse::Button::Left ) )
		camera.mouseMovementInput = input.getMouseMoveDelta();
	else
		camera.mouseMovementInput = glm::ivec2{ 0 };

	camera.keyboardZoomInput = 0.f;
	if ( input.isDown( keyboard::Key::Up ) ) camera.keyboardZoomInput -= 1.f;
	if ( input.isDown( keyboard::Key::Down ) ) camera.keyboardZoomInput += 1.f;

	camera.mouseZoomInput = input.getMouseScrollDelta();

	updateDebug( registry, input, time );
}
}  // namespace cc::app