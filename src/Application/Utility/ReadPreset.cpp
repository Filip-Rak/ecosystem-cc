#include "Application/Utility/ReadPreset.hpp"

#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <stdexcept>
#include <string_view>
#include <utility>

#include <nlohmann/json.hpp>

#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
namespace
{
using njson = nlohmann::json;

template < class F >
[[nodiscard]] auto withPath( std::string_view path, F&& function ) -> decltype( auto )
{
	try
	{
		return std::forward< F >( function )();
	}
	catch ( const njson::exception& exception )
	{
		throw std::runtime_error( "\n-> Path: " + std::string( path ) +
		                          "\n-> Issue: " + std::string( exception.what() ) );
	}
}

template < class T >
[[nodiscard]] auto get( const njson& root, std::string_view path ) -> T
{
	return withPath( path,
	                 [ & ]() -> T
	                 {
		                 const njson* current = &root;

		                 std::size_t cursor = 0;
		                 while ( true )
		                 {
			                 const auto delimiterPos = path.find( '.', cursor );
			                 const auto keyView      = path.substr( cursor, ( delimiterPos == std::string_view::npos )
			                                                                    ? ( path.size() - cursor )
			                                                                    : ( delimiterPos - cursor ) );

			                 const std::string key( keyView );
			                 current = &current->at( key.c_str() );

			                 if ( delimiterPos == std::string_view::npos ) break;
			                 cursor = delimiterPos + 1;
		                 }

		                 return current->get< T >();
	                 } );
}
}  // namespace

auto readPreset( const std::filesystem::path& path ) -> std::expected< Preset, ParsingError >
{
	std::ifstream file( path );
	if ( !file.is_open() )
	{
		return std::unexpected( "-> Couldn't open file: " + path.string() );
	}

	try
	{
		// Read
		njson json = njson::parse( file, /* callback */ nullptr, /* allowExceptions */ true, /* allowComments */ true );

		// Parse misc
		const auto gridPath        = get< std::string >( json, "gridDirectoryPath" );
		const auto iterationTarget = get< std::size_t >( json, "iterationTarget" );
		const auto rngSeed         = get< std::uint16_t >( json, "rngSeed" );

		// Parse cell - speed
		Preset::Cell::GrowthSpeed speed{
		    .base        = get< float >( json, "cell.growthSpeed.base" ),
		    .tempOptimal = get< float >( json, "cell.growthSpeed.tempOptimal" ),
		    .tempWidth   = get< float >( json, "cell.growthSpeed.tempWidth" ),
		    .humOptimal  = get< float >( json, "cell.growthSpeed.humOptimal" ),
		    .humWidth    = get< float >( json, "cell.growthSpeed.humWidth" ),
		};

		// Parse cell - limit
		Preset::Cell::GrowthLimit limit{
		    .base          = get< float >( json, "cell.growthLimit.base" ),
		    .tempOptimal   = get< float >( json, "cell.growthLimit.tempOptimal" ),
		    .tempWidth     = get< float >( json, "cell.growthLimit.tempWidth" ),
		    .humOptimal    = get< float >( json, "cell.growthLimit.humOptimal" ),
		    .humWidth      = get< float >( json, "cell.growthLimit.humWidth" ),
		    .elevHalf      = get< float >( json, "cell.growthLimit.elevHalf" ),
		    .elevSteepness = get< float >( json, "cell.growthLimit.elevSteepness" ),
		};

		Preset::Cell::Flesh Flesh{
		    .baseDecayRate  = get< float >( json, "cell.flesh.baseDecayRate" ),
		    .decayTempAccel = get< float >( json, "cell.flesh.decayTempAccel" ),
		};

		// Parse agent - environmentalSensitivity
		Preset::Agent::EnvironmentalSensitivity sensitivity{
		    .basePower                = get< float >( json, "agent.environmentalSensitivity.basePower" ),
		    .temperaturePower         = get< float >( json, "agent.environmentalSensitivity.temperaturePower" ),
		    .humidityPower            = get< float >( json, "agent.environmentalSensitivity.humidityPower" ),
		    .elevationPower           = get< float >( json, "agent.environmentalSensitivity.elevationPower" ),
		    .youthPayFactor           = get< float >( json, "agent.environmentalSensitivity.youthPayFactor" ),
		    .acclimationIncrement     = get< float >( json, "agent.environmentalSensitivity.acclimationIncrement" ),
		    .maxDrainCap              = get< float >( json, "agent.environmentalSensitivity.maxDrainCap" ),
		    .crowdPenalty             = get< float >( json, "agent.environmentalSensitivity.crowdPenalty" ),
		    .offspringSustainmentNeed = get< float >( json, "agent.environmentalSensitivity.offspringSustainmentNeed" ),
		    .energyAdaptationRate     = get< float >( json, "agent.environmentalSensitivity.energyAdaptationRate" ),
		};

		// Parse agent - modifier
		Preset::Agent::Modifier modifier{
		    .initialMutation           = get< float >( json, "agent.modifier.initialMutation" ),
		    .furtherMutations          = get< float >( json, "agent.modifier.furtherMutations" ),
		    .maxIntake                 = get< float >( json, "agent.modifier.maxIntake" ),
		    .baseEnergyBurn            = get< float >( json, "agent.modifier.baseEnergyBurn" ),
		    .baseTraversalCost         = get< float >( json, "agent.modifier.baseTraversalCost" ),
		    .climateAdaptationRate     = get< float >( json, "agent.modifier.climateAdaptationRate" ),
		    .offspringRequiredFullness = get< float >( json, "agent.modifier.offspringRequiredFulness" ),
		    .maxPerception             = get< std::size_t >( json, "agent.modifier.maxPerception" ),
		    .cellAgentLimit            = get< std::size_t >( json, "agent.modifier.cellAgentLimit" ),
		};

		// Parse agent - initialGene
		Genes initialGenes{
		    .perception            = get< std::uint8_t >( json, "agent.initialGene.perception" ),
		    .refractoryPeriod      = get< int >( json, "agent.initialGene.refractoryPeriod" ),
		    .lifespan              = get< int >( json, "agent.initialGene.lifeSpan" ),
		    .maxEnergy             = get< float >( json, "agent.initialGene.energy" ),
		    .temperaturePreference = get< float >( json, "agent.initialGene.temperaturePreference" ),
		    .humidityPreference    = get< float >( json, "agent.initialGene.humidityPreference" ),
		    .elevationPreference   = get< float >( json, "agent.initialGene.elevationPreference" ),
		    .foodPreference        = get< float >( json, "agent.initialGene.foodPreference" ),
		};

		// Construct preset
		return Preset{
		    .vegetation = { .speed = speed, .limit = limit, .flesh = Flesh },
		    .agent = { .environmentalSensitivity = sensitivity, .modifier = modifier, .initialGenes = initialGenes },
		    .presetName        = path.filename().string(),
		    .gridDirectoryPath = gridPath,
		    .iterationTarget   = iterationTarget,
		    .rngSeed           = rngSeed,
		};
	}
	catch ( const njson::parse_error& exception )
	{
		return std::unexpected( "-> Reading error \n-> " + std::string( exception.what() ) );
	}
	catch ( const std::runtime_error& exception )
	{
		return std::unexpected( "-> Parsing error " + std::string( exception.what() ) );
	}
	catch ( const std::exception& exception )
	{
		return std::unexpected( "-> Parsing error\n-> Issue: " + std::string( exception.what() ) );
	}
}
}  // namespace cc::app