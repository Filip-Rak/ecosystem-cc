#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <entt/fwd.hpp>

#include "Application/ContextEntity/Cell.hpp"

namespace cc::app
{
struct Grid
{
	const uint16_t width;
	const uint16_t height;

	std::vector< Cell > cells;
	std::vector< std::vector< entt::entity > > spatialGrid;
	const std::ptrdiff_t signedCellSize;

	Grid( entt::registry& registry, uint16_t width, uint16_t height );
};
}  // namespace cc::app