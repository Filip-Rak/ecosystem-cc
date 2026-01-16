#pragma once

#include <cstdint>
// FIXME: in ctx?
namespace cc::app
{
enum class VisModeEnum : std::uint8_t
{
	Vegetation,
	Flesh,
	Population,
	Temperature,
	Humidity,
	Elevation,
	AvgMaxEnergy
};
}