#pragma once

#include <entt/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
namespace event
{
struct ReachedTargetIteration;
}

class UISystem : public ISystem
{
public:
	UISystem( entt::registry& registry );
	auto update() -> void override;

private:
	auto onReachedTargetIteration( const event::ReachedTargetIteration& event ) -> void;

	entt::registry& m_registry;
};
}  // namespace cc::app