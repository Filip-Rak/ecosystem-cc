#pragma once

#include <fstream>
#include <optional>

#include <entt/entity/fwd.hpp>

#include "Application/Utility/Error.hpp"

namespace cc::app
{
struct AgentTickLog;
class Logger
{
public:
	Logger( entt::registry& registry );
	~Logger();

	auto init( bool clean ) -> std::optional< Error >;
	auto logTickData( const AgentTickLog& tickData ) -> void;

private:
	struct OutputData
	{
		std::ofstream file;
		std::string pendingData;
	};

	entt::registry& m_registry;
	OutputData m_tickData;
};
}  // namespace cc::app