#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include <entt/entt.hpp>

#include "Engine/Interface/ISystem.hpp"
#include "Engine/Utility/Traits.hpp"

namespace cc
{
struct IFrameService;

struct Args
{
	const std::string Title = "Engine";
	const uint16_t WindowWidth = 1280u;
	const uint16_t WindowHeight = 720u;
};

class Engine : NonMoveable, NonCopyable
{
public:
	Engine( const Args& args );

	template < typename T, typename... Args >
	    requires std::derived_from< T, ISystem >
	auto addSystem( Args&&... args ) -> T&
	{
		auto ptr = std::make_unique< T >( std::forward< Args >( args )... );
		auto& ref = *ptr;
		m_systems.push_back( std::move( ptr ) );
		return ref;
	}

	auto run() -> void;

	template < typename T >
	auto service() -> T&
	{
		return m_registry.ctx().get< T >();
	}

	auto registry() -> entt::registry&;

private:
	entt::registry m_registry;
	std::vector< std::reference_wrapper< IFrameService > > m_services;
	std::vector< std::unique_ptr< ISystem > > m_systems;
};
}  // namespace cc