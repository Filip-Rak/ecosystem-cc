#include "Application/System/Sim/AgentMovementSystem.hpp"

#include <cassert>
#include <entt/entt.hpp>

#include "Application/Components/MoveIntent.hpp"
#include "Application/Components/Position.hpp"
#include "Application/Components/Vitals.hpp"
#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
AgentMovementSystem::AgentMovementSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< Grid >() );
}

auto AgentMovementSystem::update() -> void
{
	auto& grid      = m_registry.ctx().get< Grid >();
	const auto view = m_registry.view< const component::MoveIntent, const component::Position, component::Vitals >();

	for ( const auto& [ entity, moveIntent, position, vitals ] : view.each() )
	{
		grid.moveEntity( entity, position.cellIndex, moveIntent.cellIndex );

		constexpr float energyTax = 0.05f;
		vitals.energy -= energyTax;
	}

	m_registry.remove< component::MoveIntent >( view.begin(), view.end() );
}
}  // namespace cc::app