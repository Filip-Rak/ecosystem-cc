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

Cell::Cell( float vegetation, float temperature, float elevation, float humidity,
            const Preset::Vegetation& vegetationPreset )
    : vegetation( std::max( constant::cell.minVegetation, vegetation ) ),
      temperature( temperature ),
      elevation( elevation ),
      humidity( humidity ),
      growthParameters( calculateGrowthParameters( vegetationPreset ) )
{}

auto Cell::calculateGrowthParameters( const Preset::Vegetation& preset ) const -> GrowthParameters
{
	constexpr const auto cellConstants = constant::cell;
	const auto sPreset                 = preset.speed;
	const auto lPreset                 = preset.limit;

	// Speed parameter
	const float speedTempF  = bellCurve( temperature, sPreset.tempOptimal, sPreset.tempWidth );
	const float speedHumF   = bellCurve( humidity, sPreset.humOptimal, sPreset.humWidth );
	const float speedFactor = speedTempF * speedHumF;

	// Limit parameter
	const float limitTempF = bellCurve( temperature, lPreset.tempOptimal, lPreset.tempWidth );
	const float limitHumF  = bellCurve( humidity, lPreset.humOptimal, lPreset.humWidth );
	const float limitElevF =
	    elevationPenalty( cellConstants.maxElevation, elevation, lPreset.elevHalf, lPreset.elevSteepness );

	const float limitFactor = limitTempF * limitHumF * limitElevF;

	// Effective Parameters
	return { .effectiveSpeed = speedFactor * cellConstants.growth.baseGrowthSpeed,
	         .effectiveLimit = std::clamp( limitFactor * cellConstants.growth.baseGrowthLimit,
	                                       cellConstants.minVegetation, cellConstants.maxVegetation ) };
}
}  // namespace cc::app