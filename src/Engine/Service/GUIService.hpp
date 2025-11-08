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

	GUIService();

	auto endFrame( entt::registry& registry ) -> void override;

	auto addToDraw( DrawFunction drawFunction ) -> void;
	[[nodiscard]] auto nowHandlesMouse() const -> bool;

private:
	ImGuiIO& m_IO;
	std::vector< DrawFunction > m_drawFunctions;
};
}  // namespace cc