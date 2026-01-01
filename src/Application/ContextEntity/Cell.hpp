#pragma once

#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
struct Cell
{
	struct FoodGain
	{
		const float vegetation;
		const float flesh;
	};

	struct GrowthParameters
	{
		const float effectiveSpeed;
		const float effectiveLimit;
		const float effectiveDecayRate;
	};

	float flesh{ 0.f };
	float vegetation{ 0.f };

	const float temperature;
	const float elevation;
	const float humidity;

	const GrowthParameters growthParameters;

	Cell( float vegetation, float temperature, float elevation, float humidity, const Preset::Cell& vegetationPreset );
	[[nodiscard]] auto getFoodGain( const Genes& genes ) const -> FoodGain;
	[[nodiscard]] auto getCombinedFoodGain( const Genes& genes ) const -> float;

private:
	[[nodiscard]] auto calculateGrowthParameters( const Preset::Cell& vegetationPreset ) const -> GrowthParameters;
};
}  // namespace cc::app