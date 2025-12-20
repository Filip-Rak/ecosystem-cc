#include "Application/ContextEntity/Grid.hpp"

#include <cstddef>

#include <entt/entt.hpp>

#include "Application/Components/Agent.hpp"
#include "Application/Components/GeneSet.hpp"

namespace cc::app
{
namespace
{
auto emplaceAgents( entt::registry& registry, std::vector< std::vector< entt::entity > >& spatialGrid ) -> void
{
	// TODO: const auto& basicGenes = registry.get</*preset::Genes*/>;
	const auto initialGenes   = component::GeneSet::Genes{ .maxEnergy = 100.f, .perception = 2uz };
	const auto initialGeneSet = component::GeneSet{ .agentGenes = initialGenes, .futureGenes = initialGenes };

	for ( auto index{ 0uz }; index < spatialGrid.size(); index++ )
	{
		auto& spatialCell  = spatialGrid[ index ];
		const auto& entity = spatialCell.emplace_back( registry.create() );

		registry.emplace< component::GeneSet >( entity, initialGeneSet );

		auto& agent     = registry.emplace< component::Agent >( entity );
		agent.energy    = initialGenes.maxEnergy;
		agent.cellIndex = index;
	}
}
}  // namespace

Grid::Grid( entt::registry& registry, uint16_t width, uint16_t height )
    : width( width ), height( height ), signedCellSize( static_cast< std::ptrdiff_t >( width ) * height )
{
	const auto cellCount = static_cast< std::size_t >( signedCellSize );
	cells.reserve( cellCount );

	spatialGrid.resize( cellCount );
	emplaceAgents( registry, spatialGrid );
}
}  // namespace cc::app