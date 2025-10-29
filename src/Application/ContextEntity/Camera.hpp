#pragma once

#include <glm/vec2.hpp>

namespace cc::app
{
struct Camera
{
	glm::vec2 movementInput{ 0.f };
	glm::vec2 movementOffset{ 0.f };
	float zoomInput = 0.f;
	float zoomDelta = 1.f;
};
}  // namespace cc::app
