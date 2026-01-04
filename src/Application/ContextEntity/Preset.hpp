#pragma once

#include <cstdint>
#include <filesystem>

#include "Application/Components/GeneSet.hpp"

// FIXME: This is a bad practice slowing compile times. Should be all in separate files and separate entities
namespace cc::app
{
struct Preset
{
	struct Cell
	{
		struct GrowthSpeed
		{
			const float base;

			const float tempOptimal;
			const float tempWidth;

			const float humOptimal;
			const float humWidth;
		} const speed;

		struct GrowthLimit
		{
			const float base;

			const float tempOptimal;
			const float tempWidth;

			const float humOptimal;
			const float humWidth;

			const float elevHalf;
			const float elevSteepness;
		} const limit;

		struct Flesh
		{
			const float baseDecayRate;
			const float decayTempAccel;
		} const flesh;

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
			const int refractoryMutationRate;
			const std::size_t maxPerception;
			const std::size_t cellAgentLimit;
		} const modifier;

		const Genes initialGenes;
	} const agent;

	struct Logging
	{
		const std::filesystem::path outputDirectoryPath;
	} const logging;

	const std::filesystem::path presetPath;
	const std::filesystem::path gridDirectoryPath;
	const std::size_t iterationTarget;
	const std::uint32_t rngSeed;
};
}  // namespace cc::app