#pragma once

#include <fstream>
#include <optional>

#include <entt/entity/fwd.hpp>

#include "Application/Events/SimRunnerEvents.hpp"
#include "Application/Utility/Error.hpp"

namespace cc::app
{
namespace event
{
struct ResetSim;
struct ReachedTargetIteration;
}  // namespace event

struct TickLog;
class Logger
{
public:
	Logger( entt::registry& registry );
	~Logger();

	[[nodiscard]] auto init( bool clean ) -> std::optional< Error >;
	auto logTickData( const TickLog& tickData ) -> void;

private:
	auto onResetSim( const event::ResetSim& event ) -> void;
	auto onReachedTargetIteration( const event::ReachedTargetIteration& event ) -> void;

	struct OutputData
	{
		std::ofstream file;
		std::string pendingData;
	};

	entt::registry& m_registry;
	OutputData m_tickData;
	bool m_targetReached = false;
};
}  // namespace cc::app