#include "Application/System/Sim/AgentPassingSystem.hpp"
#include "Application/Components/Destroy.hpp"
#include "Application/Components/Position.hpp"
#include "Application/ContextEntity/Grid.hpp"

#include <entt/entt.hpp>

namespace cc::app
{
AgentPassingSystem::AgentPassingSystem( entt::registry& registry ) : m_registry( registry ) {}

auto AgentPassingSystem::update() -> void
{
	auto& grid  = m_registry.ctx().get< Grid >();
	auto& cells = grid.cells();

	const auto view = m_registry.view< const component::Destroy, const component::Position >();
	for ( const auto [ entity, destroy, position ] : view.each() )
	{
		grid.removeFromSpatialGrid( entity, position.cellIndex );
		if ( destroy.reason == component::Destroy::Reason::Starvation )
		{
			cells[ position.cellIndex ].carrion += 100.f;
		}
	}

	m_registry.destroy( view.begin(), view.end() );
}
}  // namespace cc::app