#pragma once

namespace cc::app::constant
{
struct Cell_t
{
	float MinTemp = 0.f;
	float MaxTemp = 1.f;
	float MinHumidity = 0.f;
	float MaxHumidity = 1.f;
	float MinElevation = 0.f;
	float MaxElevation = 1.f;
} constexpr Cell;
}  // namespace cc::app::constant