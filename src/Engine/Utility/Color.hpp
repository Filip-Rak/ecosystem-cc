#pragma once

#include <cstdint>

namespace
{
constexpr uint8_t Opaque{ 255 };
}

namespace cc
{
struct Color
{
	uint8_t red{};
	uint8_t green{};
	uint8_t blue{};
	uint8_t alpha{ Opaque };
};
}  // namespace cc