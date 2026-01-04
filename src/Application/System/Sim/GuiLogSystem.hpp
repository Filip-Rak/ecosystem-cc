#pragma once

#include "Engine/Interface/ISystem.hpp"

#include <entt/entity/fwd.hpp>

namespace cc::app
{
namespace event
{
struct ResetSim;
}

class GuiLogSystem : public ISystem
{
public:
	GuiLogSystem( entt::registry& registry );
	auto update() -> void override;

private:
	auto onResetSim( const event::ResetSim& event ) -> void;

	entt::registry& m_registry;
};
}  // namespace cc::app