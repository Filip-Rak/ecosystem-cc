#pragma once

#include <entt/fwd.hpp>

#include "Engine/Utility/Traits.hpp"

namespace cc
{
struct IFrameService : NonMoveable, NonCopyable
{
	virtual ~IFrameService() = default;
	virtual auto beginFrame( entt::registry& registry ) -> void = 0;
	virtual auto endFrame( entt::registry& registry ) -> void = 0;
};

struct IPassiveService : IFrameService
{
	auto beginFrame( entt::registry& registry ) -> void final {}
	auto endFrame( entt::registry& registry ) -> void final {}
};

struct IBeginFrameService : IFrameService
{
	auto beginFrame( entt::registry& registry ) -> void override = 0;
	auto endFrame( entt::registry& registry ) -> void final {}
};

struct IEndFrameService : IFrameService
{
	auto beginFrame( entt::registry& registry ) -> void final {}
	auto endFrame( entt::registry& registry ) -> void override = 0;
};
}  // namespace cc
