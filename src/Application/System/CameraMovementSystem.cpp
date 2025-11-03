#include "Application/System/CameraMovementSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Constants/VisualConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Engine/Utility/Time.hpp"

namespace cc::app
{
namespace
{
// TODO: Move to visual constants
constexpr float MovementSpeed = 100.f;
constexpr float ZoomSpeed = 0.1f;
constexpr float ScrollSpeed = 0.05f;
}  // namespace
CameraMovementSystem::CameraMovementSystem( entt::registry& registry )
{
	assert( registry.ctx().contains< Camera >() && "Camera is not initialized" );
	assert( registry.ctx().contains< Time >() && "Time is not initialized" );
}

auto CameraMovementSystem::update( entt::registry& registry ) -> void
{
	constexpr const auto Constants = constant::Visual;
	auto& camera = registry.ctx().get< Camera >();
	auto& time = registry.ctx().get< Time >();

	camera.position += camera.keyboardMovementInput * time.DeltaTime * MovementSpeed;
	camera.position += camera.mouseMovementInput;

	camera.zoomLevel += camera.keyboardZoomInput * time.DeltaTime * ZoomSpeed;
	camera.zoomLevel += camera.mouseZoomInput * ScrollSpeed;  // TODO: Consider smoothing.
	camera.zoomLevel = std::clamp( camera.zoomLevel, Constants.MinZoom, Constants.MaxZoom );
}
}  // namespace cc::app