#pragma once

#include <glm/vec2.hpp>

namespace cc::app
{
struct Camera
{
	glm::vec2 keyboardMovementInput{ 0.f };
	glm::vec2 mouseMovementInput{ 0.f };
	glm::vec2 position{ 0.f };

	float keyboardZoomInput = 0.f;
	float zoomLevel = 1.f;
};
}  // namespace cc::app
