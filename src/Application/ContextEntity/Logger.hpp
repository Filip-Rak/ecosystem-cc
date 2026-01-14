#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <entt/entity/fwd.hpp>

#include "Application/Events/SimRunnerEvents.hpp"
#include "Application/Utility/Error.hpp"

namespace cc::app
{
namespace event
{
struct ResetSim;
struct ReachedTargetIteration;
struct Extinction;
}  // namespace event

struct TickLog;
struct PerformanceLog;

class Logger
{
public:
	Logger( entt::registry& registry );
	~Logger();

	[[nodiscard]] auto init( bool clean ) -> std::optional< Error >;
	auto writeTickData( const TickLog& tick ) -> void;
	auto writeTickPerformance( const PerformanceLog& perf ) -> void;

private:
	auto dumpDataAndClose() -> void;

	auto onResetSim( const event::ResetSim& event ) -> void;
	auto onReachedTargetIteration( const event::ReachedTargetIteration& event ) -> void;
	auto onExtinction( const event::Extinction& event ) -> void;

	struct OutputData
	{
		std::ofstream file;
		std::string pendingData;
		std::filesystem::path filename;
	};

	std::vector< std::unique_ptr< OutputData > > m_outputData;
	OutputData* m_tickData        = nullptr;
	OutputData* m_performanceData = nullptr;

	entt::registry& m_registry;
	bool m_collectingDone = false;
};
}  // namespace cc::app