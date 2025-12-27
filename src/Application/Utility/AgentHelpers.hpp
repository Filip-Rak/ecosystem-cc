#pragma once

#include <entt/entity/fwd.hpp>

namespace cc::app
{
struct Genes;

[[nodiscard]] auto mutateGenes( const Genes& baseGenes, float mutationOffset ) -> Genes;
auto createAgent( entt::registry& registry, const Genes& genes ) -> entt::entity;
}  // namespace cc::app