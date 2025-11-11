#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class SimulationRunnerSystem : public ISystem
{
public:
	SimulationRunnerSystem( entt::registry& registry );
	auto update( entt::registry& registry ) -> void override;

private:
	std::vector< std::unique_ptr< ISystem > > m_nestedSystems;
	float m_timeSinceLastUpdate = 0.f;
	std::size_t m_iteration = 0;
};
}  // namespace cc::app