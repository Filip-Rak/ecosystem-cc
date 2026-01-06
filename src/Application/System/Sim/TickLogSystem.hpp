#pragma once

#include <vector>

#include <entt/entity/fwd.hpp>

#include "Application/ContextEntity/TickLog.hpp"
#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
struct Grid;
struct TickLog;

class TickLogSystem : public ISystem
{
public:
	TickLogSystem( entt::registry& registry );
	auto update() -> void override;

private:
	auto logFromAgents( const Grid& grid, TickLog& tickLog ) -> void;

	entt::registry& m_registry;
	std::vector< float > m_agentEnergy;
	std::vector< float > m_agentEnergyGene;
};
}  // namespace cc::app