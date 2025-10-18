#pragma once

#include <cstdint>

namespace cc::event
{
using ExitCode = int8_t;

struct Exit
{
	ExitCode code{ 0 };
};
}  // namespace cc::event