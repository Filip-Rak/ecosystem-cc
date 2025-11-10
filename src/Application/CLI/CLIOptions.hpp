#pragma once

#include <filesystem>

namespace cc::cli
{
struct Options
{
	bool headless{ false };
	std::filesystem::path presetPath;
};
}  // namespace cc::cli