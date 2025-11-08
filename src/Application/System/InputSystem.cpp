#include "Application/System/InputSystem.hpp"

#include <cassert>
#include <iostream>  // FIXME: Debug

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

#include "Application/ContextEntity/Camera.hpp"
#include "Engine/Events/SystemEvents.hpp"
#include "Engine/Service/GUIService.hpp"
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
		std::cout << "Mouse Position { " << input.getCurrentMousePos().x << ", " << input.getCurrentMousePos().y << " }\n";
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
	assert( registry.ctx().contains< GUIService >() && "InputService not initialized" );
	assert( registry.ctx().contains< Time >() && "Time not initialized" );
	assert( registry.ctx().contains< Camera >() && "Camera not initialized" );
}

auto InputSystem::update( entt::registry& registry ) -> void
{
	const auto& input = registry.ctx().get< InputService >();
	const auto& gui = registry.ctx().get< GUIService >();
	const auto& time = registry.ctx().get< Time >();
	auto& camera = registry.ctx().get< Camera >();

	using namespace keyboard;
	using namespace mouse;

	camera.keyboardMovementInput = { 0.f, 0.f };
	if ( input.isDown( Key::W ) ) camera.keyboardMovementInput.y -= 1;
	if ( input.isDown( Key::S ) ) camera.keyboardMovementInput.y += 1;
	if ( input.isDown( Key::A ) ) camera.keyboardMovementInput.x -= 1;
	if ( input.isDown( Key::D ) ) camera.keyboardMovementInput.x += 1;

	camera.keyboardZoomInput = 0.f;
	if ( input.isDown( Key::Up ) ) camera.keyboardZoomInput -= 1.f;
	if ( input.isDown( Key::Down ) ) camera.keyboardZoomInput += 1.f;

	if ( !gui.nowHandlesMouse() )
	{
		camera.mouseZoomInput = -input.getMouseScrollDelta();
		camera.mouseMovementInput = ( input.isDown( Button::Left ) ) ? input.getMouseMoveDelta() : glm::ivec2{ 0 };
	}

	camera.isSpeedUp = input.isDown( Key::LShift );

	updateDebug( registry, input, time );
}
}  // namespace cc::app