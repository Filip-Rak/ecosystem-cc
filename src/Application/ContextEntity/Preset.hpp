#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace cc::app
{
struct Preset
{
	struct Vegetation
	{
		struct Speed
		{
			const float tempOptimal;
			const float tempWidth;

			const float humOptimal;
			const float humWidth;
		} const speed;

		struct Limit
		{
			const float tempOptimal;
			const float tempWidth;

			const float humOptimal;
			const float humWidth;

			const float elevHalf;
			const float elevSteepness;
		} const limit;
	} const vegetation;

	const std::string presetName;
	const std::filesystem::path gridDirectoryPath;
	const std::size_t iterationTarget;
	const std::uint16_t rngSeed;
};
}  // namespace cc::app