#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>

namespace cc::cli
{
namespace
{
constexpr int LogEvery10 = 10;
}
struct Options
{
	bool gui{ false };
	bool clean{ false };
	bool testPerformance{ false };
	std::filesystem::path presetPath;
	std::optional< std::filesystem::path > outputPath;
	std::optional< std::size_t > agents;
	int terminalLogfrequency = LogEvery10;
};
}  // namespace cc::cli