#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <entt/fwd.hpp>

#include "Application/ContextEntity/Cell.hpp"

namespace cc::app
{
class Grid
{
public:
	using SpatialGrid = std::vector< std::vector< entt::entity > >;

	Grid( entt::registry& registry, uint16_t width, uint16_t height );

	[[nodiscard]] auto getWidth() const -> uint16_t;
	[[nodiscard]] auto getHeight() const -> uint16_t;
	[[nodiscard]] auto getCellSize() const -> std::size_t;
	[[nodiscard]] auto getSignedCellSize() const -> uint16_t;

	[[nodiscard]] auto getCells() const -> const std::vector< Cell >&;
	[[nodiscard]] auto getSpatialGrid() const -> const SpatialGrid&;

	[[nodiscard]] auto cells() -> std::vector< Cell >&;

private:
	const uint16_t m_width;
	const uint16_t m_height;
	const std::size_t m_cellSize;
	const std::ptrdiff_t m_signedCellSize;

	std::vector< Cell > m_cells;
	SpatialGrid m_spatialGrid;
};
}  // namespace cc::app