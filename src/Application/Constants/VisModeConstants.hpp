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
	const VisMode_t Vegetation{ .Name = "Vegetation",
	                            .LowEndName = "Barren",
	                            .HighEndName = "Lush",
	                            .LowEndColor = Color{ .red = 255, .green = 255, .blue = 255 },
	                            .HighEndColor = Color{ .green = 255 } };

	const VisMode_t Elevation{ .Name = "Elevation",
	                           .LowEndName = "Lowland",
	                           .HighEndName = "Highland",
	                           .LowEndColor = Color{ .blue = 255 },
	                           .HighEndColor = Color{ .red = 139, .green = 69, .blue = 19 } };

	const VisMode_t Humidity{ .Name = "Humidity",
	                          .LowEndName = "Arid",
	                          .HighEndName = "Water",
	                          .LowEndColor = Color{ .red = 139, .green = 69, .blue = 19 },
	                          .HighEndColor = Color{ .green = 120, .blue = 255 } };

	const VisMode_t Temperature{ .Name = "Temperature",
	                             .LowEndName = "Cold",
	                             .HighEndName = "Hot",
	                             .LowEndColor = Color{ .blue = 255 },
	                             .HighEndColor = Color{ .red = 255 } };
} constexpr VisModes;
}  // namespace cc::app::constant