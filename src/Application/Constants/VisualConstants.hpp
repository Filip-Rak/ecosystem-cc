#pragma once

#include <array>
#include <cassert>
#include <string_view>

#include "Application/ContextEntity/VisMode.hpp"
#include "Engine/Utility/Color.hpp"

namespace cc::app::constant
{
struct Visual
{
	struct VisMode
	{
		const std::string_view name;
		const std::string_view lowEndName;
		const std::string_view highEndName;
		const Color lowEndColor;
		const Color highEndColor;
	};

	struct VisModes
	{
		const VisMode vegetation{ .name         = "Vegetation",
		                          .lowEndName   = "Barren",
		                          .highEndName  = "Lush",
		                          .lowEndColor  = Color{ .red = 139, .green = 69, .blue = 19 },
		                          .highEndColor = Color{ .green = 255 } };

		const VisMode population{ .name         = "Population",
		                          .lowEndName   = "Sparse",
		                          .highEndName  = "Crowded",
		                          .lowEndColor  = Color{ .red = 170, .green = 180, .blue = 120 },
		                          .highEndColor = Color{ .red = 20, .green = 120, .blue = 20 } };

		const VisMode flesh{ .name         = "Flesh",
		                     .lowEndName   = "Residual",
		                     .highEndName  = "Fresh",
		                     .lowEndColor  = Color{ .red = 220, .green = 210, .blue = 190 },
		                     .highEndColor = Color{ .red = 220, .green = 30, .blue = 30 } };

		const VisMode temperature{ .name         = "Temperature",
		                           .lowEndName   = "Cold",
		                           .highEndName  = "Hot",
		                           .lowEndColor  = Color{ .blue = 255 },
		                           .highEndColor = Color{ .red = 255 } };

		const VisMode humidity{ .name         = "Humidity",
		                        .lowEndName   = "Arid",
		                        .highEndName  = "Water",
		                        .lowEndColor  = Color{ .red = 139, .green = 69, .blue = 19 },
		                        .highEndColor = Color{ .green = 120, .blue = 255 } };

		const VisMode elevation{ .name         = "Elevation",
		                         .lowEndName   = "Lowland",
		                         .highEndName  = "Highland",
		                         .lowEndColor  = Color{ .red = 60, .green = 180, .blue = 74 },
		                         .highEndColor = Color{ .red = 139, .green = 69, .blue = 19 } };

		const VisMode avgMaxEnergy{ .name         = "Energy Gene",
		                            .lowEndName   = "Low",
		                            .highEndName  = "High",
		                            .lowEndColor  = Color{ .red = 68, .green = 1, .blue = 84 },
		                            .highEndColor = Color{ .red = 253, .green = 231, .blue = 37 } };

		const std::array< VisMode, 7 > array = { vegetation, flesh,     population,  temperature,
		                                         humidity,   elevation, avgMaxEnergy };
	} const visModes;

	const float movementSpeed     = 100.f;
	const float fastMovementSpeed = 250.f;

	const float minZoom = 0.1f;
	const float maxZoom = 3.f;

	const float zoomKeyboardSpeed     = 0.1f;
	const float fastZoomKeyboardSpeed = 0.25f;

	const float zoomScrollSpeed     = 0.05f;
	const float fastZoomScrollSpeed = 0.10f;

} inline constexpr visual;

[[nodiscard]] constexpr auto getVisModeData( VisModeEnum visMode ) -> Visual::VisMode
{
	const auto index = static_cast< std::size_t >( visMode );
	return visual.visModes.array[ index ];
}
}  // namespace cc::app::constant