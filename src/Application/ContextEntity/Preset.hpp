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
			const float speedTempOptimal;
			const float speedTempWidth;

			const float speedHumOptimal;
			const float speedHumWidth;
		} const speed;

		struct Limit
		{
			const float limitTempOptimal;
			const float limitTempWidth;

			const float limitHumOptimal;
			const float limitHumWidth;

			const float limitElevHalf;
			const float limitElevSteepness;
		} const limit;
	} const vegetation;

	const std::string presetName;
	const std::filesystem::path gridDirectoryPath;
	const std::size_t iterationTarget;
	const std::uint16_t rngSeed;
};
}  // namespace cc::app