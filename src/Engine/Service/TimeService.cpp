#include "Engine/Service/TimeService.hpp"

#include <entt/entt.hpp>

#include "Engine/Utility/Time.hpp"

namespace cc
{
auto TimeService::init( entt::registry& registry ) -> void
{
	publishTime( registry );
}

auto TimeService::beginFrame( entt::registry& registry ) -> void
{
	processDeltaTime();
	processRunTime();
	processFPS();
	publishTime( registry );
}

auto TimeService::processDeltaTime() -> void
{
	const auto currentFrameTime = clock::now();
	const auto dtTimePoint = currentFrameTime - lastFrameTime;
	const std::chrono::duration< float > dtInSeconds = dtTimePoint;
	lastFrameTime = currentFrameTime;

	m_deltaTime = dtInSeconds.count();
}

auto TimeService::processRunTime() -> void
{
	m_runTime += m_deltaTime;
}

auto TimeService::processFPS() -> void
{
	m_fps.accumulatedFrames += 1;
	m_fps.accumulatedTime += m_deltaTime;

	if ( m_fps.accumulatedTime > FPS::timeBetweenUpdates )
	{
		m_fps.lastFPS = static_cast< float >( m_fps.accumulatedFrames ) / m_fps.accumulatedTime;

		m_fps.accumulatedFrames = 0;
		m_fps.accumulatedTime = 0.f;
	}
}

auto TimeService::publishTime( entt::registry& registry ) -> void
{
	Time time{ .FPS = m_fps.lastFPS, .RunTime = m_runTime, .DeltaTime = m_deltaTime };
	registry.ctx().erase< Time >();
	registry.ctx().emplace< Time >( time );
}
}  // namespace cc