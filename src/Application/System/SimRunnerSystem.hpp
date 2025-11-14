#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "Application/ContextEntity/Grid.hpp"
#include "Engine/Interface/ISystem.hpp"

namespace cc
{
struct Time;
}

namespace cc::app
{
namespace event
{
struct ResetGrid;
}

struct SimRunnerData;

class SimRunnerSystem : public ISystem
{
public:
	SimRunnerSystem( entt::registry& registry, bool speedLimited );
	auto update() -> void override;

private:
	auto shouldUpdate( const SimRunnerData& data, const Time& time ) -> bool;
	auto onResetGrid( const event::ResetGrid& event ) -> void;

	const Grid m_initialGrid;
	const bool m_speedLimited;

	entt::registry& m_registry;
	std::vector< std::unique_ptr< ISystem > > m_nestedSystems;
	std::size_t m_iteration = 0;
	float m_timeSinceLastUpdate = 0.f;
};
}  // namespace cc::app