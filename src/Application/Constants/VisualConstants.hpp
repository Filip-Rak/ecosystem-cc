#pragma once

#include <array>
#include <cassert>
#include <string_view>

#include "Application/ContextEntity/VisMode.hpp"
#include "Engine/Utility/Color.hpp"

namespace cc::app::constant
{
struct Visual_t
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
		                            .LowEndColor = Color{ .red = 139, .green = 69, .blue = 19 },
		                            .HighEndColor = Color{ .green = 255 } };

		const VisMode_t Elevation{ .Name = "Elevation",
		                           .LowEndName = "Lowland",
		                           .HighEndName = "Highland",
		                           .LowEndColor = Color{ .red = 60, .green = 180, .blue = 74 },
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

		const VisMode_t Population{ .Name = "Population",
		                            .LowEndName = "Low",
		                            .HighEndName = "High",
		                            .LowEndColor = Color{ .red = 100, .green = 100, .blue = 100 },
		                            .HighEndColor = Color{ .red = 34, .green = 177, .blue = 76 } };

		const std::array< VisMode_t, 5 > Array = { Vegetation, Elevation, Humidity, Temperature, Population };
	} VisModes;

	const float MovementSpeed = 100.f;
	const float FastMovementSpeed = 250.f;

	const float MinZoom = 0.1f;
	const float MaxZoom = 3.f;

	const float ZoomKeyboardSpeed = 0.1f;
	const float FastZoomKeyboardSpeed = 0.25f;

	const float ZoomScrollSpeed = 0.05f;
	const float FastZoomScrollSpeed = 0.10f;

} inline constexpr Visual;

[[nodiscard]] constexpr auto getVisModeData( VisModeEnum visMode ) -> Visual_t::VisMode_t
{
	const auto index = static_cast< std::size_t >( visMode );
	return Visual.VisModes.Array[ index ];
}
}  // namespace cc::app::constant