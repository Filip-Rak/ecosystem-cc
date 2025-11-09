#pragma once

#include <optional>
#include <string>

#include <entt/fwd.hpp>

namespace cc::app
{
using ReadError = std::string;
[[nodiscard]] auto readGridFromDirectory( entt::registry& registry, const std::string& path )
    -> std::optional< ReadError >;
}  // namespace cc::app