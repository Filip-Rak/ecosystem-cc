#include "Application/App.hpp"

#include <cassert>
#include <cstdint>
#include <print>  // FIXME: Debug
#include <string_view>

#include <entt/fwd.hpp>

#include "Application/CLI/CLIOptions.hpp"
#include "Application/Constants/UIConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/UIConfig.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Application/System/CameraMovementSystem.hpp"
#include "Application/System/InputSystem.hpp"
#include "Application/System/RenderSystem.hpp"
#include "Application/System/UISystem.hpp"
#include "Application/Utility/ReadGrid.hpp"
#include "Engine/Service/SFRenderService.hpp"

namespace cc::app
{
namespace
{
constexpr uint16_t WindowWidth = 1280u;
constexpr uint16_t WindowHeight = 720u;
constexpr std::string_view Title = "Ecosystem";

auto initializeEntities( entt::registry& registry, bool headless ) -> void
{
	// TODO: Pass path from options
	// TODO: Setup should be a separate public method that can return false so main can terminate everything
	// immediately.
	const auto readingError = readGridFromDirectory( registry, "resources/Grid/" );
	if ( readingError )
	{
		// TODO: Handle outside the constructor.
		std::println( stderr, "Failed to load the grid: {}", *readingError );
		assert( false );
	}

	const auto& grid = registry.ctx().get< Grid >();

	if ( !headless )
	{
		auto& camera = registry.ctx().emplace< Camera >();
		constexpr const glm::vec2 CameraPosition{ constant::UI.SidePanel.InitialWidth / 2.f, 0.f };
		camera.position = glm::vec2{ CameraPosition };

		registry.ctx().emplace< VisualGrid >( grid.cells.size() );
		registry.ctx().emplace< UIConfig >();
	}
}
}  // namespace

App::App( const cli::Options& options )
    : m_engine( { .Title = Title.data(),
                  .WindowWidth = WindowWidth,
                  .WindowHeight = WindowHeight,
                  .EnableGUI = !options.headless } )
{
	auto& registry = m_engine.registry();
	assert( registry.ctx().contains< SFRenderService >() && "SFRenderService not initialized" );

	initializeEntities( registry, options.headless );

	if ( !options.headless )
	{
		auto& renderer = registry.ctx().get< SFRenderService >();

		m_engine.addSystem< InputSystem >( registry );
		m_engine.addSystem< CameraMovementSystem >( registry );
		m_engine.addSystem< UISystem >( registry );
		m_engine.addSystem< RenderSystem >( registry, renderer );
	}
}

auto App::run() -> void
{
	m_engine.run();
}
}  // namespace cc::app
