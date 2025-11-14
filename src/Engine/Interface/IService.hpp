#pragma once

#include <entt/fwd.hpp>

#include "Engine/Utility/Traits.hpp"

namespace cc
{
struct IFrameService : NonMoveable, NonCopyable
{
	virtual ~IFrameService() = default;
	virtual auto beginFrame() -> void = 0;
	virtual auto endFrame() -> void = 0;
};

struct IPassiveService : IFrameService
{
	auto beginFrame() -> void final {}
	auto endFrame() -> void final {}
};

struct IBeginFrameService : IFrameService
{
	auto beginFrame() -> void override = 0;
	auto endFrame() -> void final {}
};

struct IEndFrameService : IFrameService
{
	auto beginFrame() -> void final {}
	auto endFrame() -> void override = 0;
};
}  // namespace cc
