#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace cc::app
{
using InitError = std::string;

auto prepareOutputDir( const std::filesystem::path& path, bool overwrite ) -> std::optional< InitError >;
}  // namespace cc::app