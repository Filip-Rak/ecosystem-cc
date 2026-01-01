#include "Application/System/Sim/AgentFeedingSystem.hpp"

#include <algorithm>
#include <cassert>

#include <entt/entt.hpp>

#include "Application/Components/EatIntent.hpp"
#include "Application/Components/GeneSet.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Preset.hpp"

namespace cc::app
{
namespace
{
/*auto eat( float& source, float& energy, const float maxEnergy, const float maxIntake ) -> void
{
    const float hunger = std::max( 0.f, maxEnergy - energy );
    const float eaten  = std::max( std::min( { maxIntake, source, hunger } ), 0.f );

    energy += eaten;
    source -= eaten;
}*/
auto eat( float& source, float& energy, const float maxEnergy, const float maxIntake, const float population )
{
	const float hunger       = maxEnergy - energy;
	const float perAgentFood = ( population > 0 ) ? source / population : source;
	const float eaten        = std::min( { maxIntake, perAgentFood, hunger } );

	source -= eaten;
	energy += eaten;
}
}  // namespace
AgentFeedingSystem::AgentFeedingSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( m_registry.ctx().contains< Grid >() );
	assert( m_registry.ctx().contains< Preset >() );
}

auto AgentFeedingSystem::update() -> void
{
	const auto maxIntake    = m_registry.ctx().get< Preset >().agent.modifier.maxIntake;
	auto& grid              = m_registry.ctx().get< Grid >();
	const auto& spatialGrid = grid.getSpatialGrid();
	auto& cells             = grid.cells();

	const auto view = m_registry.view< const component::EatIntent, const component::Position, const component::GeneSet,
	                                   component::Vitals >();
	for ( const auto [ entity, position, geneSet, vitals ] : view.each() )
	{
		auto& cell            = cells[ position.cellIndex ];
		auto& energy          = vitals.energy;
		const auto maxEnergy  = geneSet.agentGenes.maxEnergy;
		const auto population = static_cast< float >( spatialGrid[ position.cellIndex ].size() );
		const auto foodGain   = cell.getFoodGain( geneSet.agentGenes );

		auto& foodSource = ( foodGain.flesh > foodGain.vegetation ) ? cell.flesh : cell.vegetation;
		eat( foodSource, energy, maxEnergy, maxIntake, population );
	}

	m_registry.remove< component::EatIntent >( view.begin(), view.end() );
}
}  // namespace cc::app