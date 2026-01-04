#pragma once

#include <optional>
#include <string>

namespace cc::app
{
struct Preset;
using InitError = std::string;

auto prepareOutputDir( const Preset& preset, bool overwrite ) -> std::optional< InitError >;
}  // namespace cc::app