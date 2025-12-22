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
	struct Args
	{
		entt::registry& registry;
		std::uint16_t width;
		std::uint16_t height;

		// Intended copies
		std::vector< float > temperatureValues;
		std::vector< float > humidityValues;
		std::vector< float > elevationValues;
	};

	using SpatialGrid = std::vector< std::vector< entt::entity > >;
	Grid( const Args& args );

	void moveEntity( entt::entity entity, std::size_t currentCell, std::size_t targetCell );

	[[nodiscard]] auto getWidth() const -> std::uint16_t;
	[[nodiscard]] auto getHeight() const -> std::uint16_t;
	[[nodiscard]] auto getCellCount() const -> std::size_t;
	[[nodiscard]] auto getSignedCellCount() const -> std::ptrdiff_t;

	[[nodiscard]] auto getCells() const -> const std::vector< Cell >&;
	[[nodiscard]] auto getSpatialGrid() const -> const SpatialGrid&;
	[[nodiscard]] auto copyCreationArguments() const -> Args;

	[[nodiscard]] auto cells() -> std::vector< Cell >&;

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