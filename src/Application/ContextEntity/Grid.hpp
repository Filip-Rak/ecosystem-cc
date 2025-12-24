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
	struct Position
	{
		std::ptrdiff_t x;
		std::ptrdiff_t y;
	};

	struct Args
	{
		entt::registry& registry;
		const std::uint16_t width;
		const std::uint16_t height;

		// Intended copies
		const std::vector< float > temperatureValues;
		const std::vector< float > humidityValues;
		const std::vector< float > elevationValues;
	};

	using SpatialGrid = std::vector< std::vector< entt::entity > >;
	Grid( const Args& args );

	void moveEntity( entt::entity entity, std::size_t currentCell, std::size_t targetCell );

	[[nodiscard]] auto indexToPosition( std::size_t index ) const -> Position;
	[[nodiscard]] auto PositionToIndex( ptrdiff_t x, ptrdiff_t y ) const -> std::size_t;
	[[nodiscard]] auto PositionToIndex( Position position ) const -> std::size_t;

	[[nodiscard]] auto getWidth() const -> std::uint16_t;
	[[nodiscard]] auto getHeight() const -> std::uint16_t;
	[[nodiscard]] auto getCellCount() const -> std::size_t;
	[[nodiscard]] auto getSignedCellCount() const -> std::ptrdiff_t;

	[[nodiscard]] auto getSpatialGrid() const -> const SpatialGrid&;
	[[nodiscard]] auto copyCreationArguments() const -> Args;

	[[nodiscard]] auto cells() -> std::vector< Cell >&;
	[[nodiscard]] auto cells() const -> const std::vector< Cell >&;

private:
	auto addToSpatialGrid( entt::entity entity, std::size_t cellIndex ) -> void;
	auto removeFromSpatialGrid( entt::entity targetEntity, std::size_t cellIndex ) -> void;

	const Args m_creationArguments;
	const std::uint16_t m_width;
	const std::uint16_t m_height;
	const std::size_t m_cellCount;
	const std::ptrdiff_t m_signedCellCount;
	entt::registry& m_registry;

	std::vector< Cell > m_cells;
	SpatialGrid m_spatialGrid;
};
}  // namespace cc::app