#include "Engine/Service/TimeService.hpp"

#include <cassert>

#include <entt/entt.hpp>

#include "Engine/ContextEntity/Time.hpp"

namespace cc
{
TimeService::TimeService( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< entt::dispatcher >() );
	publishTime( registry );
}

auto TimeService::beginFrame() -> void
{
	processDeltaTime();
	processRunTime();
	processFPS();
	publishTime( m_registry );
}

auto TimeService::processDeltaTime() -> void
{
	const auto currentFrameTime                      = clock::now();
	const auto dtTimePoint                           = currentFrameTime - lastFrameTime;
	const std::chrono::duration< float > dtInSeconds = dtTimePoint;
	lastFrameTime                                    = currentFrameTime;

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
		m_fps.accumulatedTime   = 0.f;
	}
}

auto TimeService::publishTime( entt::registry& registry ) -> void
{
	Time time{ .fps = m_fps.lastFPS, .runTime = m_runTime, .deltaTime = m_deltaTime };
	registry.ctx().erase< Time >();
	registry.ctx().emplace< Time >( time );
}
}  // namespace cc