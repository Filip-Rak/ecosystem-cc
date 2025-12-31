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
			const float basePower;
			const float temperaturePower;
			const float humidityPower;
			const float elevationPower;

			const float youthPayFactor;
			const float acclimationIncrement;
			const float maxDrainCap;

			const float crowdPenalty;
			const float offspringSustainmentNeed;

			const float energyAdaptationRate;
		} const environmentalSensitivity;

		struct Modifier
		{
			const float initialMutation;
			const float furtherMutations;
			const float maxIntake;
			const float baseEnergyBurn;
			const float baseTraversalCost;
			const float climateAdaptationRate;
			const float offspringRequiredFullness;
			const std::size_t maxPerception;
			const std::size_t cellAgentLimit;
		} const modifier;

		const Genes initialGenes;
	} const agent;

	const std::string presetName;
	const std::filesystem::path gridDirectoryPath;
	const std::size_t iterationTarget;
	const std::uint16_t rngSeed;
};
}  // namespace cc::app