#pragma once

#include <optional>

#include <entt/entity/fwd.hpp>

#include "Application/Utility/Error.hpp"

namespace cc::app
{
class Logger
{
public:
	Logger( entt::registry& registry );
	auto init( bool clean ) -> std::optional< Error >;

private:
	entt::registry& m_registry;
};
}  // namespace cc::app