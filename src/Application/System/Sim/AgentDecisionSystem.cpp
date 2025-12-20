#include "Application/System/Sim/AgentDecisionSystem.hpp"

#include <cassert>

#include <cstddef>
#include <entt/entt.hpp>

#include "Application/Components/Agent.hpp"
#include "Application/Components/GeneSet.hpp"
#include "Application/Components/MoveIntent.hpp"
#include "Application/ContextEntity/Cell.hpp"
#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
namespace
{
constexpr std::size_t maxPerception = 3;  // TODO: Put in preset.

auto rangeOffsets( const Grid& grid, std::size_t range ) -> std::vector< std::ptrdiff_t >
{
	std::vector< std::ptrdiff_t > offsets;
	offsets.reserve( ( static_cast< std::size_t >( ( 2uz * range ) + 1uz ) ) * ( 2uz * range + 1uz ) );

	const auto signedRange = static_cast< std::ptrdiff_t >( range );
	for ( auto dy = -signedRange; dy <= signedRange; dy++ )
	{
		for ( auto dx = -signedRange; dx <= signedRange; dx++ )
		{
			const auto index = ( dy * grid.width ) + dx;
			offsets.emplace_back( index );
		}
	}

	return offsets;
}
}  // namespace

AgentDecisionSystem::AgentDecisionSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Grid >() );
	auto& grid = registry.ctx().get< Grid >();

	for ( auto range = 0uz; range < maxPerception; range++ )
	{
		m_rangeOffsets.emplace_back( rangeOffsets( grid, range ) );
	}
}

auto AgentDecisionSystem::update() -> void
{
	auto& grid = m_registry.ctx().get< Grid >();
	auto view  = m_registry.view< component::Agent, component::GeneSet >();

	for ( auto entity : view )
	{
		const auto& genes = view.get< component::GeneSet >( entity ).agentGenes;
		auto& agent       = view.get< component::Agent >( entity );

		// TODO: Expand.
		// Pick cell with most food in range.
		const auto bestIndex = bestCell( grid, genes.perception, agent.cellIndex );
		if ( agent.cellIndex != bestIndex )
		{
			m_registry.emplace_or_replace< component::MoveIntent >( entity, bestIndex );
		}
	}
}

auto AgentDecisionSystem::bestCell( const Grid& grid, std::size_t perception, std::size_t cellIndex ) -> std::size_t
{
	const auto& cells = grid.cells;

	assert( perception <= maxPerception );
	assert( cellIndex < cells.size() );

	std::size_t bestCell = cellIndex;

	for ( const auto offset : m_rangeOffsets[ perception ] )
	{
		const auto newIndex = static_cast< std::ptrdiff_t >( cellIndex ) + offset;
		if ( newIndex >= grid.signedCellSize || newIndex < 0 )
		{
			continue;
		}

		if ( cells[ newIndex ].vegetation > cells[ bestCell ].vegetation )
		{
			bestCell = newIndex;
		}
	}

	return bestCell;
}
}  // namespace cc::app