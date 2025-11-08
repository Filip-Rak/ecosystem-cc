// Copied from SFML/Window/Keyboard.hpp
#pragma once

#include <array>

#include <glm/vec2.hpp>
#include <magic_enum/magic_enum.hpp>

#include "Engine/Utility/InputEnums.hpp"

namespace cc
{
struct InputMap
{
	std::array< bool, magic_enum::enum_count< keyboard::Key >() > keySates{ false };
	std::array< bool, magic_enum::enum_count< mouse::Button >() > buttonSates{ false };
	glm::ivec2 mousePos{ 0 };
	float mouseScrollDelta{ 0.f };
	bool windowInFocus = true;
};
}  // namespace cc