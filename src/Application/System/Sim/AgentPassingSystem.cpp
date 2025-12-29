#include "Application/System/Sim/AgentPassingSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/Components/Destroy.hpp"
#include "Application/Components/GeneSet.hpp"
#include "Application/Components/MoveIntent.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/SimLog.hpp"

namespace cc::app
{
AgentPassingSystem::AgentPassingSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( m_registry.ctx().contains< Grid >() );
	assert( m_registry.ctx().contains< Preset >() );
	assert( m_registry.ctx().contains< SimLog >() );
}

auto AgentPassingSystem::update() -> void
{
	const auto& preset = m_registry.ctx().get< Preset >();

	const auto vitalsView = m_registry.view< const component::GeneSet, component::Vitals >();
	for ( const auto& [ entity, geneSet, vitals ] : vitalsView.each() )
	{
		const auto& agentGenes = geneSet.agentGenes;

		using enum component::Destroy::Reason;
		if ( vitals.energy <= 0.f )
		{
			m_registry.emplace< component::Destroy >( entity, Starvation );
		}
		else if ( vitals.age >= agentGenes.lifespan )
		{
			m_registry.emplace< component::Destroy >( entity, Age );
		}

		vitals.age++;
		vitals.remainingRefractoryPeriod--;

		if ( m_registry.any_of< component::MoveIntent >( entity ) )
		{
			vitals.energy -= preset.agent.modifier.baseEnergyBurn;
		}
	}

	auto& simLog = m_registry.ctx().get< SimLog >();
	auto& grid   = m_registry.ctx().get< Grid >();
	auto& cells  = grid.cells();

	const auto destroyView = m_registry.view< const component::Destroy, const component::Position >();
	for ( const auto [ entity, destroy, position ] : destroyView.each() )
	{
		grid.removeFromSpatialGrid( entity, position.cellIndex );
		cells[ position.cellIndex ].carrion += 100.f;

		using enum component::Destroy::Reason;
		switch ( destroy.reason )
		{
		case Starvation: simLog.starvations++; break;
		case Age: simLog.oldAgeDeaths++; break;
		default: assert( false );
		}

		simLog.currentPopulation--;
	}

	m_registry.destroy( destroyView.begin(), destroyView.end() );
}
}  // namespace cc::app