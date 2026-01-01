#include "Application/System/CameraMovementSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Constants/VisualConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Engine/ContextEntity/Time.hpp"

namespace cc::app
{
CameraMovementSystem::CameraMovementSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Camera >() );
	assert( registry.ctx().contains< Time >() );
}

auto CameraMovementSystem::update() -> void
{
	constexpr const auto& Constants = constant::visual;
	const auto& time                = m_registry.ctx().get< Time >();
	auto& camera                    = m_registry.ctx().get< Camera >();

	float zoomKeyboardSpeed = Constants.ZoomKeyboardSpeed;
	float zoomScrollSpeed   = Constants.ZoomScrollSpeed;
	float movementSpeed     = Constants.MovementSpeed;

	if ( camera.isSpedUp )
	{
		zoomKeyboardSpeed = Constants.FastZoomKeyboardSpeed;
		zoomScrollSpeed   = Constants.FastZoomScrollSpeed;
		movementSpeed     = Constants.FastMovementSpeed;
	}

	const float keyboardZoom = camera.keyboardZoomInput * time.DeltaTime * zoomKeyboardSpeed;
	const float mouseZoom    = camera.mouseZoomInput * zoomScrollSpeed;

	const float newZoom = camera.zoomLevel + keyboardZoom + mouseZoom;
	camera.zoomLevel    = std::clamp( newZoom, Constants.MinZoom, Constants.MaxZoom );

	const glm::vec2 keyboardMove = camera.keyboardMovementInput * time.DeltaTime * movementSpeed / camera.zoomLevel;
	const glm::vec2 mouseMove    = camera.mouseMovementInput / camera.zoomLevel;
	camera.position += keyboardMove + mouseMove;
}
}  // namespace cc::app