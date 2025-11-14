#pragma once

#include <entt/fwd.hpp>

#include "Engine/Utility/Traits.hpp"

namespace cc
{
struct ISystem : NonCopyable, NonMoveable
{
	virtual auto update() -> void = 0;
	virtual ~ISystem() = default;
};
}  // namespace cc