#pragma once

#include <optional>

#include "Application/CLI/CLIOptions.hpp"
#include "Application/Utility/Error.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Utility/Traits.hpp"

namespace cc::cli
{
struct Options;
}

namespace cc::app
{
struct Preset;
class App : NonMoveable, NonCopyable
{
public:
	App( const cli::Options& options );
	[[nodiscard]] auto init() -> std::optional< Error >;

	auto run() -> void;

private:
	[[nodiscard]] auto initEntities( entt::registry& registry, const Preset& preset ) const -> std::optional< Error >;
	auto initSystems() -> void;

	const cli::Options m_cliOptions;
	Engine m_engine;
};
}  // namespace cc::app
