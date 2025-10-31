#pragma once

#include <string_view>

#include "Engine/Utility/Color.hpp"

namespace cc::app::constant
{
struct VisMode_t
{
	const std::string_view Name;
	const std::string_view LowEndName;
	const std::string_view HighEndName;
	const Color LowEndColor;
	const Color HighEndColor;
};

struct VisModes_t
{
	const VisMode_t Temperature{ .Name = "Temperature",
	                             .LowEndName = "Cold",
	                             .HighEndName = "Hot",
	                             .LowEndColor = Color{ .blue = 255 },
	                             .HighEndColor = Color{ .red = 255 } };
} constexpr VisModes;
}  // namespace cc::app::constant