#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <entt/entity/entity.hpp>

#include "Application/Grid/Cell.hpp"

namespace cc::app
{

struct Grid
{
	const uint16_t Width;
	const uint16_t Height;

	std::vector< Cell > cellsRowMajor;
	std::vector< std::vector< entt::entity > > spatialGrid;

	Grid( uint16_t width, uint16_t height ) : Width( width ), Height( height )
	{
		const auto CellCount = static_cast< std::size_t >( width ) * height;
		cellsRowMajor.reserve( CellCount );
		spatialGrid.reserve( CellCount );
	}
};
}  // namespace cc::app