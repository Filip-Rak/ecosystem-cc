#pragma once

#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
struct Cell
{
	struct GrowthParameters
	{
		const float effectiveSpeed;
		const float effectiveLimit;
	};

	float vegetation;

	const float temperature;
	const float elevation;
	const float humidity;
	const GrowthParameters growthParameters;

	Cell( float vegetation, float temperature, float elevation, float humidity,
	      const Preset::Vegetation& vegetationPreset );

private:
	[[nodiscard]] auto calculateGrowthParameters( const Preset::Vegetation& vegetationPreset ) const
	    -> GrowthParameters;
};
}  // namespace cc::app