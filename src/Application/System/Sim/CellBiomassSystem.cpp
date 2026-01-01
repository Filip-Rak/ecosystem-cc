#include "Application/System/Sim/CellBiomassSystem.hpp"

#include <algorithm>
#include <cassert>
#include <entt/entt.hpp>

#include "Application/Constants/CellConstants.hpp"
#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
CellBiomassSystem::CellBiomassSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Grid >() );
}

auto CellBiomassSystem::update() -> void
{
	constexpr const auto constants = constant::cell;
	auto& gridCells                = m_registry.ctx().get< Grid >().cells();

	for ( Cell& cell : gridCells )
	{
		const auto params = cell.growthParameters;

		float& vegetation   = cell.vegetation;
		const float vegRate = constants.maxVegetation - ( vegetation / params.effectiveLimit );
		vegetation += params.effectiveSpeed * vegetation * vegRate;
		vegetation = std::clamp( vegetation, constants.minVegetation, params.effectiveLimit );

		float& flesh = cell.flesh;
		flesh -= params.effectiveDecayRate * flesh;
		flesh = std::clamp( flesh, constants.minFlesh, constants.maxFlesh );
	}
}
}  // namespace cc::app