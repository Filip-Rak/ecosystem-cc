#include "Application/ContextEntity/Cell.hpp"

#include <cmath>
#include <cstdlib>

#include "Application/Constants/CellConstants.hpp"

namespace cc::app
{
Cell::Cell( float vegetation, float temperature, float elevation, float humidity )
    : vegetation( vegetation ),
      temperature( temperature ),
      elevation( elevation ),
      humidity( humidity ),
      growthParameters( calculateGrowthParameters() )
{}

auto Cell::calculateGrowthParameters() const -> GrowthParameters
{
	constexpr const auto& Constants = constant::Cell;
	constexpr const auto& Growth = Constants.Growth;

	const float tempDiff = std::abs( temperature - Growth.IdealTemperature );
	const float tempDiffNormalized = tempDiff / Constants.TemperatureRange;
	const float tempMod = std::pow( 1.f - tempDiffNormalized, Growth.TemperaturePenalty );

	const float humDiff = std::abs( humidity - Growth.IdealHumidity );
	const float humDiffNormalized = humDiff / Constants.HumidityRange;
	const float humMod = std::pow( 1.f - humDiffNormalized, Growth.HumidityPenalty );

	const float elevDiff = std::abs( elevation - Growth.IdealElevationLimit );
	const float elevDiffNormalized = elevDiff / Constants.ElevationRange;
	const float elevMod = ( elevation > Growth.IdealElevationLimit )
	                          ? std::pow( Constants.ElevationRange - elevDiff, Growth.ElevationPenalty )
	                          : 1.f;

	const float combinedModifier = tempMod * humMod * elevMod;
	return { .speedFactor = Growth.SpeedFactor * combinedModifier,
	         .growthLimit = ( combinedModifier * Constants.VegetationRange ) + Constants.MinVegetation };
}
}  // namespace cc::app