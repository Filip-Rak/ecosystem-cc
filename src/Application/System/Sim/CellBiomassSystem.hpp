#pragma once

#include <entt/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
class CellBiomassSystem : public ISystem
{
public:
	CellBiomassSystem( entt::registry& registry );
	auto update() -> void override;

private:
	entt::registry& m_registry;
};
}  // namespace cc::app