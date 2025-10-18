#pragma once

#include <cstdint>

#include <glm/vec2.hpp>

#include "Engine/Utility/InputMap.hpp"

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
	glm::vec2 position;
	bool pressed;
};
}  // namespace cc::event