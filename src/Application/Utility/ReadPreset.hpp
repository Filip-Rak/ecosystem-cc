#pragma once

#include <expected>
#include <filesystem>
#include <string>

namespace cc::app
{
struct Preset;
using ParsingError = std::string;

[[nodiscard]] auto readPreset( const std::filesystem::path& path ) -> std::expected< Preset, ParsingError >;
}  // namespace cc::app