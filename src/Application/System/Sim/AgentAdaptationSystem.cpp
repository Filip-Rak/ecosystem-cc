#include "Application/System/Sim/AgentAdaptationSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Components/FailedToMate.hpp"
#include "Application/Components/GeneSet.hpp"
#include "Application/Components/OffspringIntent.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
namespace
{
auto tickTowards( float& value, const float targetValue, const float adaptationRate ) -> void
{
	value += std::clamp( targetValue - value, -adaptationRate, adaptationRate );
}
}  // namespace

AgentAdaptationSystem::AgentAdaptationSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( m_registry.ctx().contains< Grid >() );
	assert( m_registry.ctx().contains< Preset >() );
}

auto AgentAdaptationSystem::update() -> void
{
	const auto& cells  = m_registry.ctx().get< Grid >().cells();
	const auto& preset = m_registry.ctx().get< Preset >();

	const auto view = m_registry.view< const component::Position, const component::Vitals, component::GeneSet >();
	for ( const auto [ entity, position, vitals, geneSet ] : view.each() )
	{
		auto& futureGenes = geneSet.futureGenes;
		const auto& cell  = cells[ position.cellIndex ];

		const float climateRate = preset.agent.modifier.climateAdaptationRate;
		tickTowards( futureGenes.temperaturePreference, cell.temperature, climateRate );
		tickTowards( futureGenes.elevationPreference, cell.elevation, climateRate );
		tickTowards( futureGenes.humidityPreference, cell.humidity, climateRate );

		constexpr float energyRate = 0.01f;
		auto& maxEnergy            = geneSet.futureGenes.maxEnergy;
		if ( m_registry.any_of< component::OffspringIntent >( entity ) )
			maxEnergy += energyRate;
		else if ( m_registry.any_of< component::FailedToMate >( entity ) )
			maxEnergy -= energyRate;
	}

	const auto toDelete = m_registry.view< component::FailedToMate >();
	m_registry.erase< component::FailedToMate >( toDelete.begin(), toDelete.end() );
}
};  // namespace cc::app