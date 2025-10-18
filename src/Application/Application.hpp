#pragma once

#include "Engine/Engine.hpp"
#include "Engine/Utility/Traits.hpp"

namespace cc::eco
{
class Application : NonMoveable, NonCopyable
{
public:
	Application();
	auto run() -> void;

private:
	Engine m_engine;
};
}  // namespace cc::eco
