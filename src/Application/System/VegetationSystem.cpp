#include "Application/System/VegetationSystem.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
VegetationSystem::VegetationSystem( entt::registry& registry )
{
	assert( registry.ctx().contains< Grid >() && "Grid not initialized" );
}

auto VegetationSystem::update( entt::registry& registry ) -> void
{
	auto& gridCells = registry.ctx().get< Grid >().cells;
	for ( Cell& cell : gridCells )
	{
		auto& vegetation = cell.vegetation;
		const auto& speedFactor = cell.GrowthParameters.SpeedFactor;
		const auto& growthLimit = cell.GrowthParameters.GrowthLimit;

		vegetation = std::min( vegetation + speedFactor, growthLimit );
	}
}
}  // namespace cc::app