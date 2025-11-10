#pragma once

#include <filesystem>

namespace cc::cli
{
struct Options
{
	bool gui{ false };
	std::filesystem::path presetPath;
};
}  // namespace cc::cli