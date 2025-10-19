#pragma once

#include <chrono>

#include <entt/fwd.hpp>

#include "Engine/Interface/IService.hpp"

namespace
{
using clock = std::chrono::steady_clock;
constexpr float CloseToZero = 0.001f;
}  // namespace

namespace cc
{
class TimeService : public IBeginFrameService
{
public:
	auto init( entt::registry& registry ) -> void override;
	auto beginFrame( entt::registry& registry ) -> void override;

private:
	struct FPS
	{
		static constexpr float timeBetweenUpdates = 1.f;
		float accumulatedTime = 0.f;
		int accumulatedFrames = 0;

		float lastFPS = 0.f;
	};

	auto processDeltaTime() -> void;
	auto processRunTime() -> void;
	auto processFPS() -> void;
	auto publishTime( entt::registry& registry ) -> void;

	clock::time_point lastFrameTime = clock::now();
	float m_deltaTime = CloseToZero;
	float m_runTime = CloseToZero;

	FPS m_fps;
};
}  // namespace cc