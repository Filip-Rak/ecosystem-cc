#pragma once

#include <cstddef>
#include <cstdint>

#include <entt/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class AgentDecisionSystem : public ISystem
{
public:
	AgentDecisionSystem( entt::registry& registry );
	auto update() -> void override;

private:
	std::vector< std::vector< std::ptrdiff_t > > m_rangeOffsets;
	std::vector< std::uint8_t > m_moveIntentions;
	entt::registry& m_registry;
	const std::size_t m_maxPerception;
};
}  // namespace cc::app