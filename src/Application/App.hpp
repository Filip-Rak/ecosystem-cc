#pragma once

#include <optional>
#include <string>

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
	auto init( const cli::Options& options ) -> std::optional< InitError >;

	auto run() -> void;

private:
	Engine m_engine;
};
}  // namespace cc::app
