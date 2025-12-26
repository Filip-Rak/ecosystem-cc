#pragma once

#include <cstdint>

namespace cc::app::component
{
struct Destroy
{
	enum class Reason : std::uint8_t
	{
		Starvation
	} reason;
};
}  // namespace cc::app::component