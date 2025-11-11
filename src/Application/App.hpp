#pragma once

#include <optional>
#include <string>

#include "Application/CLI/CLIOptions.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Utility/Traits.hpp"

namespace cc::cli
{
struct Options;
}

namespace cc::app
{
using InitError = std::string;

class App : NonMoveable, NonCopyable
{
public:
	App( const cli::Options& options );
	[[nodiscard]] auto init() -> std::optional< InitError >;

	auto run() -> void;

private:
	auto initSystems() -> void;

	const cli::Options m_CliOptions;
	Engine m_engine;
};
}  // namespace cc::app
