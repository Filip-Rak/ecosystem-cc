#pragma once

namespace cc::app
{
struct Cell
{
	float vegetation;

	const float Temperature;
	const float Elevation;
	const float Humidity;

	Cell( float vegetation, float temperature, float elevation, float humidity )
	    : vegetation( vegetation ), Temperature( temperature ), Elevation( elevation ), Humidity( humidity )
	{}
};
}  // namespace cc::app