#pragma once

#include <cstddef>

#include <entt/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
struct Grid;

class AgentDecisionSystem : public ISystem
{
public:
	AgentDecisionSystem( entt::registry& registry );
	auto update() -> void override;

private:
	auto bestCell( const Grid& grid, std::size_t perception, std::size_t cellIndex ) -> std::size_t;

	std::vector< std::vector< std::ptrdiff_t > > m_rangeOffsets;  // TODO: Either dynamic vector or fixed std::array
	entt::registry& m_registry;
};
}  // namespace cc::app