#pragma once

#include <chrono>

#include <entt/fwd.hpp>

#include "Engine/Interface/IService.hpp"

namespace
{
using clock                 = std::chrono::steady_clock;
constexpr float closeToZero = 0.000404f;
}  // namespace

namespace cc
{
class TimeService : public IBeginFrameService
{
public:
	TimeService( entt::registry& registry );
	auto beginFrame() -> void override;

private:
	struct FPS
	{
		static constexpr float timeBetweenUpdates = 1.f;
		float accumulatedTime                     = 0.f;
		int accumulatedFrames                     = 0;

		float lastFPS = 0.f;
	};

	auto processDeltaTime() -> void;
	auto processRunTime() -> void;
	auto processFPS() -> void;
	auto publishTime( entt::registry& registry ) -> void;

	entt::registry& m_registry;
	clock::time_point lastFrameTime = clock::now();
	float m_deltaTime               = closeToZero;
	float m_runTime                 = closeToZero;

	FPS m_fps;
};
}  // namespace cc