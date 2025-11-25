#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <entt/entt.hpp>

#include "Application/Components/Position.hpp"
#include "Application/ContextEntity/Cell.hpp"

namespace cc::app
{
struct Grid
{
	const uint16_t Width;
	const uint16_t Height;

	std::vector< Cell > cells;
	std::vector< std::vector< entt::entity > > spatialGrid;

	Grid( entt::registry& registry, uint16_t width, uint16_t height ) : Width( width ), Height( height )
	{
		const auto CellCount = static_cast< std::size_t >( width ) * height;
		cells.reserve( CellCount );

		spatialGrid.resize( CellCount );
		for ( auto& spatialCell : spatialGrid )
		{
			const auto& entity = spatialCell.emplace_back( registry.create() );
			registry.emplace< component::Position >( entity );
		}
	}
};
}  // namespace cc::app