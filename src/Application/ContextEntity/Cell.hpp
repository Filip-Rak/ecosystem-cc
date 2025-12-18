#pragma once

namespace cc::app
{
struct Cell
{
	struct GrowthParameters
	{
		const float speedFactor;
		const float growthLimit;
	};

	float vegetation;

	const float temperature;
	const float elevation;
	const float humidity;
	const GrowthParameters growthParameters;

	Cell( float vegetation, float temperature, float elevation, float humidity );

private:
	[[nodiscard]] auto calculateGrowthParameters() const -> GrowthParameters;
};
}  // namespace cc::app