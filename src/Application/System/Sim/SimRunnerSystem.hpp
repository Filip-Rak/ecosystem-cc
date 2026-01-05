#pragma once

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
	auto triggerEvents() -> void;
	auto shouldUpdate( const SimRunnerData& data, const Time& time ) -> bool;
	auto onResetSim( const event::ResetSim& event ) -> void;

	const bool m_inGui;
	bool m_blockExecution       = false;
	float m_timeSinceLastUpdate = 0.f;
	entt::registry& m_registry;
	std::vector< std::unique_ptr< ISystem > > m_simSystems;
};
}  // namespace cc::app