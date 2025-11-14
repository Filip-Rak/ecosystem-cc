#pragma once

#include <entt/fwd.hpp>

#include "Engine/Utility/Traits.hpp"

// FIXME: In hindsight I feel like having all the services (and systems) registry as an argument through their
// interfaces was a suboptimal decision. Since I already went ahead with objective approach to systems, I always had
// that availability of making a system take the registry  reference in the constructor and deciding what to do with it
// on it's onw from there. Some services do not need the registry at all. Others, as it turns out, may in fact have a
// reason to save the registry as a member reference anyway as they may need it in order to handle event callbacks. At
// this point, having the registry be an argument for frame & update interface methods arbitrarily forces every instance
// to have more complicated prototypes for no reason adding extra confusion with what to do when we don't have access to
// the registry in callback methods.
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
