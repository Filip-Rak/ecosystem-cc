#include "Application/System/CameraMovementSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/ContextEntity/Camera.hpp"
#include "Engine/Utility/Time.hpp"

namespace cc::app
{
namespace
{
constexpr float MovementSpeed = 100.f;
constexpr float ZoomSpeed = 0.1f;
}  // namespace
CameraMovementSystem::CameraMovementSystem( entt::registry& registry )
{
	assert( registry.ctx().contains< Camera >() && "Camera is not initialized" );
	assert( registry.ctx().contains< Time >() && "Time is not initialized" );
}

auto CameraMovementSystem::update( entt::registry& registry ) -> void
{
	auto& camera = registry.ctx().get< Camera >();
	auto& time = registry.ctx().get< Time >();

	camera.movementOffset = camera.movementInput * time.DeltaTime * MovementSpeed;
	camera.zoomDelta = camera.zoomInput * time.DeltaTime * ZoomSpeed;
}
}  // namespace cc::app