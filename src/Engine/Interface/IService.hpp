#pragma once

#include <entt/fwd.hpp>

#include "Engine/Utility/Traits.hpp"

namespace cc
{
struct IFrameService : NonMoveable, NonCopyable
{
	virtual ~IFrameService() = default;
	virtual void init( entt::registry& registry ) {}
	virtual void beginFrame( entt::registry& registry ) = 0;
	virtual void endFrame( entt::registry& registry ) = 0;
};

struct IPassiveService : IFrameService
{
	void beginFrame( entt::registry& registry ) final {}
	void endFrame( entt::registry& registry ) final {}
};

struct IBeginFrameService : IFrameService
{
	void beginFrame( entt::registry& registry ) override = 0;
	void endFrame( entt::registry& registry ) final {}
};

struct IEndFrameService : IFrameService
{
	void beginFrame( entt::registry& registry ) final {}
	void endFrame( entt::registry& registry ) override = 0;
};
}  // namespace cc
