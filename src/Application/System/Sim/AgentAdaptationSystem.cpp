#include "Application/System/Sim/AgentAdaptationSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
namespace
{
auto tickTowards( float& preference, const float cellValue, const float adaptationRate ) -> void
{
	preference += std::clamp( cellValue - preference, -adaptationRate, adaptationRate );
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

	const auto view = m_registry.view< const component::Position, component::GeneSet >();
	for ( const auto [ entity, position, geneSet ] : view.each() )
	{
		auto& futureGenes = geneSet.futureGenes;
		const auto& cell  = cells[ position.cellIndex ];

		{
			const float rate = preset.agent.modifier.climateAdaptationRate;
			tickTowards( futureGenes.temperaturePreference, cell.temperature, rate );
			tickTowards( futureGenes.elevationPreference, cell.elevation, rate );
			tickTowards( futureGenes.humidityPreference, cell.humidity, rate );
		}
	}
}
};  // namespace cc::app