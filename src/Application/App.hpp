#pragma once

#include <optional>

#include "Application/CLI/CLIOptions.hpp"
#include "Application/Events/SimRunnerEvents.hpp"
#include "Application/Utility/Error.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Utility/Traits.hpp"

namespace cc::cli
{
struct Options;
}

namespace cc::app
{
namespace event
{
struct ReachedTargetIteration;
struct Extinction;
}  // namespace event

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

	auto closeGracefully() -> void;

	auto onTargetReached( const event::ReachedTargetIteration& event ) -> void;
	auto onExtinction( const event::Extinction& event ) -> void;

	const cli::Options m_cliOptions;
	Engine m_engine;
};
}  // namespace cc::app
