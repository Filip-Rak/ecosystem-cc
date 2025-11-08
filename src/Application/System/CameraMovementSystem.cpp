#include "Application/System/CameraMovementSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Constants/VisualConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Engine/Utility/Time.hpp"

namespace cc::app
{
CameraMovementSystem::CameraMovementSystem( entt::registry& registry )
{
	assert( registry.ctx().contains< Camera >() && "Camera is not initialized" );
	assert( registry.ctx().contains< Time >() && "Time is not initialized" );
}

auto CameraMovementSystem::update( entt::registry& registry ) -> void
{
	constexpr const auto& Constants = constant::Visual;
	auto& camera = registry.ctx().get< Camera >();
	auto& time = registry.ctx().get< Time >();

	float zoomKeyboardSpeed = Constants.ZoomKeyboardSpeed;
	float zoomScrollSpeed = Constants.ZoomScrollSpeed;
	float movementSpeed = Constants.MovementSpeed;

	if ( camera.isSpeedUp )
	{
		zoomKeyboardSpeed = Constants.FastZoomKeyboardSpeed;
		zoomScrollSpeed = Constants.FastZoomScrollSpeed;
		movementSpeed = Constants.FastMovementSpeed;
	}

	// TODO: Consider smoothing.
	const float keyboardZoom = camera.keyboardZoomInput * time.DeltaTime * zoomKeyboardSpeed;
	const float mouseZoom = camera.mouseZoomInput * zoomScrollSpeed;

	const float newZoom = camera.zoomLevel + keyboardZoom + mouseZoom;
	camera.zoomLevel = std::clamp( newZoom, Constants.MinZoom, Constants.MaxZoom );

	const glm::vec2 keyboardMove = camera.keyboardMovementInput * time.DeltaTime * movementSpeed * camera.zoomLevel;
	const glm::vec2 mouseMove = camera.mouseMovementInput * camera.zoomLevel;
	camera.position += keyboardMove + mouseMove;
}
}  // namespace cc::app