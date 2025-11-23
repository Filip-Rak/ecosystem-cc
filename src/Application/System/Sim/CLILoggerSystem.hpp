#pragma once

#include <entt/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class CLILoggerSystem : public ISystem
{
public:
	CLILoggerSystem( entt::registry& registry, int logInfrequency );
	auto update() -> void override;

private:
	const int m_logInfrequency;
	entt::registry& m_registry;
};
}  // namespace cc::app