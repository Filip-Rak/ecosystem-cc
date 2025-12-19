#pragma once

namespace cc::app::constant
{
struct CellConstants
{
	const float minVegetation = 1e-6f;
	const float maxVegetation = 1.f;

	const float minTemperature = 0.f;
	const float maxTemperature = 1.f;

	const float minHumidity = 0.f;
	const float maxHumidity = 1.f;

	const float minElevation = 0.f;
	const float maxElevation = 1.f;

	const float vegetationRange  = maxVegetation - minVegetation;
	const float temperatureRange = maxTemperature - minTemperature;
	const float humidityRange    = maxHumidity - minHumidity;
	const float elevationRange   = maxElevation - minElevation;
} inline constexpr cell;
}  // namespace cc::app::constant