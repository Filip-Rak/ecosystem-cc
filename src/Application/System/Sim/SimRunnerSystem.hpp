#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include <entt/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc
{
struct Time;
namespace cli
{
struct Options;
}
}  // namespace cc

namespace cc::app
{
namespace event
{
struct ResetSim;
}

struct SimRunnerData;

class SimRunnerSystem : public ISystem
{
public:
	SimRunnerSystem( entt::registry& registry, const cli::Options& CLIOptions );
	auto update() -> void override;

private:
	auto shouldUpdate( const SimRunnerData& data, const Time& time ) -> bool;
	auto onResetSim( const event::ResetSim& event ) -> void;

	const bool m_speedLimited;
	entt::registry& m_registry;
	std::vector< std::unique_ptr< ISystem > > m_simSystems;
	std::size_t m_iteration     = 0;
	float m_timeSinceLastUpdate = 0.f;
};
}  // namespace cc::app