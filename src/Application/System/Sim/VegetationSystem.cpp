#include "Application/System/Sim/VegetationSystem.hpp"

#include <cassert>
#include <entt/entt.hpp>

#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
VegetationSystem::VegetationSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Grid >() );
}

auto VegetationSystem::update() -> void
{
	auto& gridCells = m_registry.ctx().get< Grid >().cells;
	for ( Cell& cell : gridCells )
	{
		auto& vegetation = cell.vegetation;
		const auto& speedFactor = cell.growthParameters.speedFactor;
		const auto& growthLimit = cell.growthParameters.growthLimit;

		vegetation = std::min( vegetation + speedFactor, growthLimit );
	}
}
}  // namespace cc::app