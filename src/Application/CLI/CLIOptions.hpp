#pragma once

#include <filesystem>

namespace cc::cli
{
namespace
{
constexpr int LogEvery10 = 10;
}
struct Options
{
	bool gui{ false };
	bool overwrite{ false };
	std::filesystem::path presetPath;
	int terminalLogfrequency = LogEvery10;
};
}  // namespace cc::cli