#include "Application/System/Sim/VegetationSystem.hpp"

#include <cassert>
#include <entt/entt.hpp>

#include "Application/Constants/CellConstants.hpp"
#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
VegetationSystem::VegetationSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Grid >() );
}

auto VegetationSystem::update() -> void
{
	constexpr const auto constants = constant::cell;
	auto& gridCells                = m_registry.ctx().get< Grid >().cells;

	for ( Cell& cell : gridCells )
	{
		const auto params = cell.growthParameters;
		float& vegetation = cell.vegetation;

		vegetation += params.effectiveSpeed * vegetation * ( constants.maxVegetation - vegetation / params.effectiveLimit );
		vegetation = std::clamp( vegetation, constants.minVegetation, params.effectiveLimit );
	}
}
}  // namespace cc::app