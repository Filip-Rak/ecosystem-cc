#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace cc::app
{
struct Preset
{
	const std::string presetName;
	const std::filesystem::path gridDirectoryPath;
	const std::size_t iterationTarget;
	const std::uint16_t rngSeed;
};
}  // namespace cc::app