#pragma once

#include <entt/entity/fwd.hpp>

namespace cc::app
{
struct Genes;

auto createAgent( entt::registry& registry, const Genes& genes ) -> entt::entity;
auto mutateGenes( const Genes& baseGenes, float mutationOffset ) -> Genes;
}  // namespace cc::app