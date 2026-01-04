#pragma once

#include "Application/Utility/Error.hpp"

#include <expected>
#include <filesystem>

namespace cc::app
{
struct Preset;
[[nodiscard]] auto readPreset( const std::filesystem::path& path ) -> std::expected< Preset, Error >;
}  // namespace cc::app