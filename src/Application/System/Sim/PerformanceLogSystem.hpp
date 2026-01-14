#include "Engine/Interface/ISystem.hpp"

#include <entt/fwd.hpp>

#include <cstddef>

namespace cc::app
{
class PerformanceLogSystem : public ISystem
{
public:
	PerformanceLogSystem( entt::registry& registry );
	auto update() -> void override;

private:
	entt::registry& m_registry;
	std::size_t m_previousAgentCount{};
	bool m_firstFrame = true;
};
}  // namespace cc::app