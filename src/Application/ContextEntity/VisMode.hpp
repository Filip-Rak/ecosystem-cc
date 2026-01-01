#pragma once

#include <cstdint>

namespace cc::app
{
enum class VisModeEnum : std::uint8_t
{
	Vegetation,
	Flesh,
	Population,
	Temperature,
	Humidity,
	Elevation
};
}