#pragma once

#include <cstdint>

namespace cc::app::component
{
struct JustEaten
{
	enum class Source : std::uint8_t
	{
		Vegetation,
		Flesh
	};

	const Source source;
	const float amount;
};
}  // namespace cc::app::component