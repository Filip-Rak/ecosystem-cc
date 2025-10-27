#pragma once

namespace cc::app
{
struct Cell
{
	float vegetation;

	const float Elevation;
	const float Humidity;
	const float Temperature;

	Cell( float vegetation, float elevation, float humidity, float temperature )
	    : vegetation( vegetation ),
	      Elevation( elevation ),
	      Humidity( humidity ),
	      Temperature( temperature )
	{}
};
}  // namespace cc::app