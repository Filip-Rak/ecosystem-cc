#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

#include "Application/Components/GeneSet.hpp"

namespace cc::app
{
struct Preset
{
	struct Vegetation
	{
		struct Speed
		{
			const float base;

			const float tempOptimal;
			const float tempWidth;

			const float humOptimal;
			const float humWidth;
		} const speed;

		struct Limit
		{
			const float base;

			const float tempOptimal;
			const float tempWidth;

			const float humOptimal;
			const float humWidth;

			const float elevHalf;
			const float elevSteepness;
		} const limit;
	} const vegetation;

	struct Agent
	{
		struct EnvironmentalSensitivity
		{
			const float base;

			const float temperature;
			const float humidity;
			const float elevation;
		} const environmentalSensitivity;

		struct Modifier
		{
			float initialMutation;
			float furtherMutations;
			float maxIntake;
			float baseEnergyBurn;
			float baseTraversalCost;
			float climateAdaptationRate;
			std::size_t maxPerception;
		} const modifier;

		const Genes initialGenes;
	} const agent;

	const std::string presetName;
	const std::filesystem::path gridDirectoryPath;
	const std::size_t iterationTarget;
	const std::uint16_t rngSeed;
};
}  // namespace cc::app