#pragma once

namespace cc::app::constant
{
struct Cell_t
{
	const float MinTemp = 0.f;
	const float MaxTemp = 1.f;
	const float MinHumidity = 0.f;
	const float MaxHumidity = 1.f;
	const float MinElevation = 0.f;
	const float MaxElevation = 1.f;
} constexpr Cell;
}  // namespace cc::app::constant