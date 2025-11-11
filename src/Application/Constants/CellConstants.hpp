#pragma once

namespace cc::app::constant
{
struct Cell_t
{
	struct Growth_t
	{
		const float SpeedFactor = 0.001f;

		const float IdealTemperature = 0.5f;
		const float TemperaturePenalty = 3.f;

		const float IdealHumidity = 0.7f;
		const float HumidityPenalty = 3.f;

		const float IdealElevationLimit = 0.7f;
		const float ElevationPenalty = 6.f;
	} const Growth;

	const float MinVegetation = 0.f;
	const float MaxVegetation = 1.f;

	const float MinTemperature = 0.f;
	const float MaxTemperature = 1.f;

	const float MinHumidity = 0.f;
	const float MaxHumidity = 1.f;

	const float MinElevation = 0.f;
	const float MaxElevation = 1.f;

	const float VegetationRange = MaxVegetation - MinVegetation;
	const float TemperatureRange = MaxTemperature - MinTemperature;
	const float HumidityRange = MaxHumidity - MinHumidity;
	const float ElevationRange = MaxElevation - MinElevation;
} inline constexpr Cell;
}  // namespace cc::app::constant