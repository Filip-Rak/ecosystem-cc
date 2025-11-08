#pragma once

namespace cc::app::constant
{
struct Cell_t
{
	const float MinVegetation = 0.f;
	const float MaxVegetation = 1.f;

	const float MinTemperature = 0.f;
	const float MaxTemperature = 1.f;

	const float MinHumidity = 0.f;
	const float MaxHumidity = 1.f;

	const float MinElevation = 0.f;
	const float MaxElevation = 1.f;

	const float VegetationRange = MinVegetation - MaxVegetation;
	const float TemperatureRange = MinTemperature - MaxTemperature;
	const float HumidityRange = MinHumidity - MaxHumidity;
	const float ElevationRange = MinElevation - MaxElevation;
} inline constexpr Cell;
}  // namespace cc::app::constant