#pragma once

#include <memory>
#include <utility>
#include <vector>

#include <entt/entt.hpp>

#include "Engine/Interface/IService.hpp"
#include "Engine/Interface/ISystem.hpp"
#include "Engine/Utility/Traits.hpp"

namespace cc
{
namespace event
{
struct Exit;
}

struct Args
{
	const std::string Title = "Engine";
	const uint16_t WindowWidth = 1280u;
	const uint16_t WindowHeight = 720u;
	const bool EnableGUI = true;
};

class Engine : NonMoveable, NonCopyable
{
public:
	Engine( const Args& args );

	auto run() -> void;

	template < typename T, typename... Args >
	requires std::derived_from< T, ISystem >
	auto addSystem( Args&&... args ) -> T&
	{
		auto ptr = std::make_unique< T >( std::forward< Args >( args )... );
		auto& ref = *ptr;
		m_systems.push_back( std::move( ptr ) );
		return ref;
	}

	[[nodiscard]] auto registry() -> entt::registry&;

private:
	template < typename T, typename... Args >
	requires std::derived_from< T, IFrameService >
	auto addService( Args&&... args ) -> T&
	{
		auto ptr = std::make_unique< T >( std::forward< Args >( args )... );
		auto& ref = *ptr;
		m_registry.ctx().emplace< T& >( ref );
		m_services.push_back( std::move( ptr ) );

		return ref;
	}

	auto initWindowEntities() -> void;
	auto setupDispatcher() -> void;
	auto createCoreServices() -> void;
	auto createWindowServices( const Args& args ) -> void;

	auto onExit( const event::Exit& exitEvent ) -> void;

	entt::registry m_registry;
	std::vector< std::unique_ptr< IFrameService > > m_services;
	std::vector< std::unique_ptr< ISystem > > m_systems;

	bool m_isRunning{ true };
};
}  // namespace cc