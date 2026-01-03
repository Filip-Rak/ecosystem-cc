#pragma once

#include <entt/entity/fwd.hpp>

namespace cc::app
{
struct Genes;
auto createAgent( entt::registry& registry, const Genes& genes, float energy ) -> entt::entity;
}  // namespace cc::app