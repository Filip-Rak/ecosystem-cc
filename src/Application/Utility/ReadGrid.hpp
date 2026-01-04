#pragma once

#include <expected>
#include <filesystem>

#include <entt/fwd.hpp>

#include "Application/ContextEntity/Grid.hpp"
#include "Application/Utility/Error.hpp"

namespace cc::app
{
[[nodiscard]] auto readGridFromDirectory( entt::registry& registry, const std::filesystem::path& path )
    -> std::expected< Grid::Args, Error >;
}  // namespace cc::app