#pragma once

#include <cstdint>

namespace cc::app
{
enum class VisModeEnum : uint8_t
{
	Vegetation,
	Elevation,
	Humidity,
	Temperature,
};
}