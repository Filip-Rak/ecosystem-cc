#pragma once

#include <vector>

#include <entt/entity/fwd.hpp>

#include "Engine/Interface/ISystem.hpp"

namespace cc::app
{
struct Genes;
class AgentOffspringSystem : public ISystem
{
public:
	AgentOffspringSystem( entt::registry& m_registry );
	auto update() -> void override;

private:
	struct CreationArgs
	{
		const Genes& parentFutureGenes;
		const std::size_t position;
		const float energy;
	};

	std::vector< CreationArgs > m_agentsToCreate;
	entt::registry& m_registry;
};
}  // namespace cc::app