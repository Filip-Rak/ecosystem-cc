// Copied from SFML/Window/Keyboard.hpp
#pragma once

#include <array>
#include <cstdint>

#include <glm/vec2.hpp>
#include <magic_enum/magic_enum.hpp>

namespace cc
{
namespace keyboard
{
enum class Key : std::uint8_t
{
	Unknown = 0,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	Num0,
	Num1,
	Num2,
	Num3,
	Num4,
	Num5,
	Num6,
	Num7,
	Num8,
	Num9,
	Escape,
	LControl,
	LShift,
	LAlt,
	LSystem,
	RControl,
	RShift,
	RAlt,
	RSystem,
	Menu,
	LBracket,
	RBracket,
	Semicolon,
	Comma,
	Period,
	Apostrophe,
	Slash,
	Backslash,
	Grave,
	Equal,
	Hyphen,
	Space,
	Enter,
	Backspace,
	Tab,
	PageUp,
	PageDown,
	End,
	Home,
	Insert,
	Delete,
	Add,
	Subtract,
	Multiply,
	Divide,
	Left,
	Right,
	Up,
	Down,
	Numpad0,
	Numpad1,
	Numpad2,
	Numpad3,
	Numpad4,
	Numpad5,
	Numpad6,
	Numpad7,
	Numpad8,
	Numpad9,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	Pause
};
}  // namespace keyboard
namespace mouse
{
enum Button : uint8_t
{
	Left = 0,
	Right,
	Middle,
	Unknown
};
}

struct InputMap
{
	std::array< bool, magic_enum::enum_count< keyboard::Key >() > keySates{ false };
	std::array< bool, magic_enum::enum_count< mouse::Button >() > buttonSates{ false };
	glm::vec2 mousePos{ 0.f };
};
}  // namespace cc