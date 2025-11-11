#pragma once

namespace cc::app
{
struct Cell
{
	struct GrowthParameters_t
	{
		const float SpeedFactor;
		const float GrowthLimit;
	};

	float vegetation;

	const float Temperature;
	const float Elevation;
	const float Humidity;
	const GrowthParameters_t GrowthParameters;

	Cell( float vegetation, float temperature, float elevation, float humidity );

private:
	[[nodiscard]] auto calculateGrowthParameters() const -> GrowthParameters_t;
};
}  // namespace cc::app