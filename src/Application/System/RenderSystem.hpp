#pragma once

#include "Engine/Interface/IRenderService.hpp"
#include "Engine/Interface/ISystem.hpp"
#include "Engine/Utility/RenderHandles.hpp"

namespace cc::app
{
class RenderSystem : public ISystem
{
public:
	RenderSystem( entt::registry& registry, IRenderService& renderer );
	auto update( entt::registry& registry ) -> void override;

private:
	IRenderService& m_renderer;
	GridHandle m_grid;
};
}  // namespace cc::app