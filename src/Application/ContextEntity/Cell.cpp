#include "Application/ContextEntity/Cell.hpp"

#include <algorithm>
#include <cmath>

#include "Application/Constants/CellConstants.hpp"

namespace cc::app
{
namespace
{
inline auto bellCurve( float value, float optimal, float width ) -> float
{
	const float alignment = ( value - optimal ) / std::max( 1e-6f, width );
	const float result    = std::exp( -0.5f * alignment * alignment );
	return result;
}

inline auto elevationPenalty( float maxElevation, float elevation, float half, float steepness ) -> float
{
	return maxElevation / ( maxElevation + std::exp( steepness * ( elevation - half ) ) );
}

}  // namespace

Cell::Cell( float vegetation, float temperature, float elevation, float humidity )
    : vegetation( std::max( constant::cell.minVegetation, vegetation ) ),
      temperature( temperature ),
      elevation( elevation ),
      humidity( humidity ),
      growthParameters( calculateGrowthParameters() )
{}

auto Cell::calculateGrowthParameters() const -> GrowthParameters
{
	constexpr const auto constants = constant::cell;

	// Speed parameter
	constexpr float speedTempOptimal = 0.6f;
	constexpr float speedTempWidth   = 0.25f;

	constexpr float speedHumOptimal = 0.65f;
	constexpr float speedHumWidth   = 0.20f;

	const float speedTempF  = bellCurve( temperature, speedTempOptimal, speedTempWidth );
	const float speedHumF   = bellCurve( humidity, speedHumOptimal, speedHumWidth );
	const float speedFactor = speedTempF * speedHumF;

	// Limit parameter
	constexpr float limitTempOptimal = 0.6f;
	constexpr float limitTempWidth   = 0.27f;

	constexpr float limitHumOptimal = 0.65f;
	constexpr float limitHumWidth   = 0.3f;

	constexpr float limitElevHalf      = 0.7f;
	constexpr float limitElevSteepness = 6.f;

	const float limitTempF  = bellCurve( temperature, limitTempOptimal, limitTempWidth );
	const float limitHumF   = bellCurve( humidity, limitHumOptimal, limitHumWidth );
	const float limitElevF  = elevationPenalty( constants.maxElevation, elevation, limitElevHalf, limitElevSteepness );
	const float limitFactor = limitTempF * limitHumF * limitElevF;

	return { .effectiveSpeed = speedFactor * constants.growth.baseGrowthSpeed,
	         .effectiveLimit = limitFactor * constants.growth.baseGrowthLimit };
}
}  // namespace cc::app