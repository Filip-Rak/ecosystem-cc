#include "Application/System/Sim/SimLogSystem.hpp"

#include <cassert>

#include <cstdlib>
#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/SimLog.hpp"

namespace cc::app
{
SimLogSystem::SimLogSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< SimLog >() );
	assert( registry.ctx().contains< Grid >() );
}

auto SimLogSystem::update() -> void
{
	auto& simLog      = m_registry.ctx().get< SimLog >();
	const auto& grid  = m_registry.ctx().get< Grid >();
	const auto& cells = grid.cells();

	auto agentCount           = 0uz;
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
		foodPrefSum += genes.foodPreference;
	}

	if ( agentCount > 0uz )
	{
		const auto fAgentCount   = static_cast< float >( agentCount );
		simLog.averageEnergy     = maxEnergySum / fAgentCount;
		simLog.averageAdaptation = adaptationRatingSum / fAgentCount;
		simLog.averageFoodPref   = foodPrefSum / fAgentCount;
	}
}
};  // namespace cc::app