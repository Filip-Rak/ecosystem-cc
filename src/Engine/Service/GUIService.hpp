#pragma once

#include <functional>

#include <entt/fwd.hpp>

#include "Engine/Interface/IService.hpp"

struct ImGuiIO;

namespace cc
{
class GUIService : public IEndFrameService
{
public:
	using DrawFunction = std::function< void( entt::registry& registry ) >;

	GUIService( entt::registry& registry );

	auto endFrame() -> void override;
	auto addToDraw( DrawFunction drawFunction ) -> void;
	auto rebuildFont() const -> void;

	[[nodiscard]] auto nowHandlesMouse() const -> bool;

private:
	entt::registry& m_registry;
	ImGuiIO& m_IO;
	std::vector< DrawFunction > m_drawFunctions;
};
}  // namespace cc