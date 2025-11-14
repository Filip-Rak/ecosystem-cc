#pragma once

#include <expected>
#include <filesystem>
#include <string>

#include <entt/fwd.hpp>

namespace cc::app
{
struct Preset
{
	const std::filesystem::path gridDirectoryPath;
	const std::size_t iterationTarget;
};

using ParsingError = std::string;

[[nodiscard]] auto readPreset( const std::filesystem::path& path ) -> std::expected< Preset, ParsingError >;
}  // namespace cc::app