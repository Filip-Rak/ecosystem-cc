#include "Application/ContextEntity/Cell.hpp"

#include <algorithm>
#include <cmath>

#include "Application/Constants/CellConstants.hpp"

namespace cc::app
{
namespace
{
auto bellCurve( float value, float optimal, float width ) -> float
{
	const float alignment = ( value - optimal ) / std::max( 1e-6f, width );
	const float result    = std::exp( -0.5f * alignment * alignment );

	return result;
}

auto elevationPenalty( float maxElevation, float elevation, float half, float steepness ) -> float
{
	return maxElevation / ( maxElevation + std::exp( steepness * ( elevation - half ) ) );
}

}  // namespace

Cell::Cell( float vegetation, float temperature, float elevation, float humidity, const Preset::Cell& vegetationPreset )
    : vegetation( std::max( constant::cell.minVegetation, vegetation ) ),
      temperature( temperature ),
      elevation( elevation ),
      humidity( humidity ),
      growthParameters( calculateGrowthParameters( vegetationPreset ) )
{
	this->vegetation = growthParameters.effectiveLimit;
}

auto Cell::getFoodGain( const Genes& genes ) const -> FoodGain
{
	const float foodPref       = genes.foodPreference;
	const float vegetationGain = vegetation * ( 1.f - foodPref );
	const float fleshGain      = flesh * foodPref;

	return { .vegetation = vegetationGain, .flesh = fleshGain };
}

auto Cell::getCombinedFoodGain( const Genes& genes ) const -> float
{
	const auto gain = getFoodGain( genes );
	return gain.vegetation + gain.flesh;
}

auto Cell::calculateGrowthParameters( const Preset::Cell& preset ) const -> GrowthParameters
{
	constexpr const auto cellConstants = constant::cell;
	const auto speedPreset             = preset.speed;
	const auto limitPreset             = preset.limit;

	// Speed parameter
	const float speedTempF     = bellCurve( temperature, speedPreset.tempOptimal, speedPreset.tempWidth );
	const float speedHumF      = bellCurve( humidity, speedPreset.humOptimal, speedPreset.humWidth );
	const float speedF         = speedTempF * speedHumF;
	const float effectiveSpeed = speedF * speedPreset.base;

	// Limit parameter
	const float limitTempF = bellCurve( temperature, limitPreset.tempOptimal, limitPreset.tempWidth );
	const float limitHumF  = bellCurve( humidity, limitPreset.humOptimal, limitPreset.humWidth );
	const float limitElevF =
	    elevationPenalty( cellConstants.maxElevation, elevation, limitPreset.elevHalf, limitPreset.elevSteepness );

	const float limitFactor = limitTempF * limitHumF * limitElevF;
	const float effectiveLimit =
	    std::clamp( limitFactor * limitPreset.base, cellConstants.minVegetation, cellConstants.maxVegetation );

	// Flesh
	const auto& flesh             = preset.flesh;
	const auto baseDecayRate      = flesh.baseDecayRate;
	const auto decayTempAccel     = flesh.decayTempAccel;
	const auto effectiveDecayRate = baseDecayRate + ( decayTempAccel * temperature );

	// Effective Parameters
	return {
	    .effectiveSpeed = effectiveSpeed, .effectiveLimit = effectiveLimit, .effectiveDecayRate = effectiveDecayRate };
}
}  // namespace cc::app