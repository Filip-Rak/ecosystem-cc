#pragma once

#include <entt/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class CLILoggerSystem : public ISystem
{
public:
	CLILoggerSystem( entt::registry& registry, int logfrequency );
	auto update() -> void override;

private:
	const int m_logfrequency;
	entt::registry& m_registry;
};
}  // namespace cc::app