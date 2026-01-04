#include "Application/System/Sim/GuiLogSystem.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/GuiLog.hpp"
#include "Application/ContextEntity/TickDataCollection.hpp"
#include "Application/Events/SimRunnerEvents.hpp"

namespace cc::app
{
GuiLogSystem::GuiLogSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< TickDataCollection >() );
	assert( registry.ctx().contains< GuiLog >() );
	assert( registry.ctx().contains< Grid >() );

	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::ResetSim >().connect< &GuiLogSystem::onResetSim >( *this );
}

auto GuiLogSystem::update() -> void
{
	const auto& grid  = m_registry.ctx().get< Grid >();
	const auto& cells = grid.cells();
	auto& guiLog      = m_registry.ctx().get< GuiLog >();
	auto& tickData    = m_registry.ctx().get< TickDataCollection >();

	guiLog.totalBirths += tickData.births;
	guiLog.totalStarvations += tickData.starvations;
	guiLog.oldAgeDeaths += tickData.oldAgeDeaths;
	guiLog.totalVegetationEaten += tickData.vegetationEaten;

	auto agentCount           = 0uz;
	int refractoryPeriodSum   = 0;
	float maxEnergySum        = 0.f;
	float adaptationRatingSum = 0.f;
	float foodPrefSum         = 0.f;

	const auto& view =
	    m_registry.view< const component::GeneSet, const component::Vitals, const component::Position >();
	for ( const auto& [ entity, geneSet, vitals, position ] : view.each() )
	{
		agentCount++;
		maxEnergySum += geneSet.agentGenes.maxEnergy;

		const auto& cell  = cells[ position.cellIndex ];
		const auto& genes = geneSet.agentGenes;

		const float tDiff = std::abs( cell.temperature - genes.temperaturePreference );
		const float hDiff = std::abs( cell.humidity - genes.humidityPreference );
		const float eDiff = std::abs( cell.elevation - genes.elevationPreference );

		const float adaptation = 1 - ( ( tDiff + hDiff + eDiff ) / 3.f );
		adaptationRatingSum += adaptation;
		refractoryPeriodSum += genes.refractoryPeriod;
		foodPrefSum += genes.foodPreference;
	}

	if ( agentCount > 0uz )
	{
		const auto fAgentCount         = static_cast< float >( agentCount );
		guiLog.averageRefractoryPeriod = static_cast< float >( refractoryPeriodSum ) / fAgentCount;
		guiLog.averageEnergyGene       = maxEnergySum / fAgentCount;
		guiLog.averageAdaptation       = adaptationRatingSum / fAgentCount;
		guiLog.averageFoodPref         = foodPrefSum / fAgentCount;
	}

	guiLog.currentPopulation = agentCount;
	guiLog.highestPopulation = std::max( guiLog.currentPopulation, guiLog.highestPopulation );
}

auto GuiLogSystem::onResetSim( const event::ResetSim& /*event*/ ) -> void
{
	auto& guiLog = m_registry.ctx().get< GuiLog >();
	guiLog       = GuiLog{};
}
};  // namespace cc::app