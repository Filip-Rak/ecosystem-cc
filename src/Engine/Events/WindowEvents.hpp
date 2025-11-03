#pragma once

#include <cstdint>

#include <glm/vec2.hpp>

#include "Engine/ContextEntity/InputMap.hpp"  // TODO: Remove

namespace cc::event
{
struct WindowClosed
{};

struct LostFocus
{};

struct GainedFocus
{};

struct WindowResized
{
	uint16_t width;
	uint16_t height;
};

struct KeyChanged
{
	cc::keyboard::Key key;
	bool pressed;
};

struct MouseButtonChanged
{
	mouse::Button button;
	glm::ivec2 position;
	bool pressed;
};

struct MouseWheelMoved
{
	glm::ivec2 position{ 0 };
	float scrollDelta = 0;
};
}  // namespace cc::event