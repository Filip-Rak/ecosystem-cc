#pragma once

#include "Engine/Engine.hpp"
#include "Engine/Utility/Traits.hpp"

namespace cc::cli
{
struct Params;
}

namespace cc::eco
{
class App : NonMoveable, NonCopyable
{
public:
	App( const cli::Params& params );
	auto run() -> void;

private:
	Engine m_engine;
};
}  // namespace cc::eco
