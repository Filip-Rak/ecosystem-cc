#include "Application/System/Sim/AgentDecisionSystem.hpp"

#include <cassert>

#include <cstddef>
#include <entt/entt.hpp>

#include "Application/Components/GeneSet.hpp"
#include "Application/Components/NextMove.hpp"
#include "Application/Components/Position.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "entt/entity/fwd.hpp"

namespace cc::app
{
namespace
{
constexpr std::size_t maxPerception = 3;  // TODO: Put in preset.

// TODO: Cheybyshev - consider Manhattan for better performance.
auto rangeOffsets( const Grid& grid, std::size_t range ) -> std::vector< std::ptrdiff_t >
{
	std::vector< std::ptrdiff_t > offsets;
	const auto side = ( 2uz * range ) + 1uz;
	offsets.reserve( side * side );

	const auto signedRange = static_cast< std::ptrdiff_t >( range );
	for ( auto dy = -signedRange; dy <= signedRange; dy++ )
	{
		for ( auto dx = -signedRange; dx <= signedRange; dx++ )
		{
			const auto index = ( dy * grid.getWidth() ) + dx;
			offsets.emplace_back( index );
		}
	}

	return offsets;
}

// TODO: Cheybyshev - consider Manhattan for better performance.
auto nextStepUnsafe( std::ptrdiff_t gridWith, std::ptrdiff_t pos, std::ptrdiff_t target ) -> std::size_t
{
	// TODO: Make a helper in Grid.hpp
	auto x = pos / gridWith;
	auto y = pos & gridWith;

	const auto targetX = target & gridWith;
	const auto targetY = target / gridWith;

	const auto deltaX = targetX - x;
	const auto deltaY = targetY - y;

	if ( deltaX != 0 ) x += ( deltaX > 0 ) ? 1 : -1;
	if ( deltaY != 0 ) y += ( deltaY > 0 ) ? 1 : -1;

	// TODO: Make a helper in Grid.hpp
	return ( y * gridWith ) + x;
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

// TODO: Signed, unsigned is a costly mess. Figure out if full switch to signed is feasible or minimize
auto AgentDecisionSystem::update() -> void
{
	auto& grid      = m_registry.ctx().get< Grid >();
	const auto view = m_registry.view< component::Position, component::GeneSet >();

	for ( const auto& [ entity, position, geneSet ] : view.each() )
	{
		// TODO: Expand.
		// Pick cell with most food in range.

		// Returns unsigned and its immediately casted to signed.
		const auto bestIndex = bestCell( grid, geneSet.agentGenes.perception, position.cellIndex );
		if ( position.cellIndex != bestIndex )
		{
			const auto signedIndex     = static_cast< ptrdiff_t >( position.cellIndex );
			const auto signedBestIndex = static_cast< ptrdiff_t >( bestIndex );
			const auto stepIndex       = nextStepUnsafe( grid.getWidth(), signedIndex, signedBestIndex );

			m_registry.emplace_or_replace< component::NextMove >( entity, stepIndex );
		}
	}
}

auto AgentDecisionSystem::bestCell( const Grid& grid, std::size_t perception, std::size_t cellIndex ) -> std::size_t
{
	const auto& cells = grid.getCells();

	assert( perception <= maxPerception );
	assert( cellIndex < cells.size() );

	std::size_t bestCell = cellIndex;

	for ( const auto offset : m_rangeOffsets[ perception - 1 ] )
	{
		const auto newIndex = static_cast< std::ptrdiff_t >( cellIndex ) + offset;

		// Clip at borders
		if ( newIndex >= grid.getSignedCellSize() || newIndex < 0 )
		{
			continue;
		}

		const auto unsignedNI = static_cast< std::size_t >( newIndex );
		if ( cells[ unsignedNI ].vegetation > cells[ bestCell ].vegetation )
		{
			bestCell = unsignedNI;
		}
	}

	return bestCell;
}
}  // namespace cc::app