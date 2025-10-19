#pragma once

#include "Engine/Engine.hpp"
#include "Engine/Utility/Traits.hpp"

namespace cc::cli
{
struct Options;
}

namespace cc::eco
{
class App : NonMoveable, NonCopyable
{
public:
	App( const cli::Options& options );
	auto run() -> void;

private:
	Engine m_engine;
};
}  // namespace cc::eco
