#include "Application/ContextEntity/Grid.hpp"

#include <cstddef>

#include <entt/entt.hpp>

#include "Application/Components/Agent.hpp"
#include "Application/Components/Genes.hpp"

namespace cc::app
{
namespace
{
auto emplaceAgents( entt::registry& registry, std::vector< std::vector< entt::entity > >& spatialGrid ) -> void
{
	// TODO: const auto& basicGenes = registry.get</*preset::Genes*/>;
	const auto basicGenes = component::Genes{ .maxEnergy = 100.f, .perception = 2uz };

	for ( auto index{ 0uz }; index < spatialGrid.size(); index++ )
	{
		auto& spatialCell  = spatialGrid[ index ];
		const auto& entity = spatialCell.emplace_back( registry.create() );

		registry.emplace< component::Genes >( entity, basicGenes );

		auto& agent     = registry.emplace< component::Agent >( entity );
		agent.energy    = basicGenes.maxEnergy;
		agent.cellIndex = index;
	}
}
}  // namespace

Grid::Grid( entt::registry& registry, uint16_t width, uint16_t height ) : width( width ), height( height )
{
	const auto cellCount = static_cast< std::size_t >( width ) * height;
	cells.reserve( cellCount );

	spatialGrid.resize( cellCount );
	emplaceAgents( registry, spatialGrid );
}
}  // namespace cc::app