#pragma once

#include <expected>
#include <filesystem>
#include <string>

#include <entt/fwd.hpp>

#include "Application/ContextEntity/Grid.hpp"

namespace cc::app
{
using ReadError = std::string;
[[nodiscard]] auto readGridFromDirectory( entt::registry& registry, const std::filesystem::path& path )
    -> std::expected< Grid::Args, ReadError >;
}  // namespace cc::app