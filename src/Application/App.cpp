#include "Application/App.hpp"

#include <cassert>
#include <cstdint>
#include <string_view>

#include <entt/fwd.hpp>

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
#include "Application/Utility/ReadPreset.hpp"
#include "Engine/Service/SFRenderService.hpp"

namespace cc::app
{
namespace
{
constexpr uint16_t WindowWidth = 1280u;
constexpr uint16_t WindowHeight = 720u;
constexpr std::string_view Title = "Ecosystem";

[[nodiscard]] auto initializeEntities( entt::registry& registry, const cli::Options& options, const Preset& preset )
    -> std::optional< InitError >
{
	const auto readingError = readGridFromDirectory( registry, preset.gridDirectoryPath );
	if ( readingError )
	{
		return "-> Couldn't load the grid\n" + *readingError;
	}

	const auto& grid = registry.ctx().get< Grid >();

	if ( options.gui )
	{
		auto& camera = registry.ctx().emplace< Camera >();
		constexpr const glm::vec2 CameraPosition{ constant::UI.SidePanel.InitialWidth / 2.f, 0.f };
		camera.position = glm::vec2{ CameraPosition };

		registry.ctx().emplace< VisualGrid >( grid.cells.size() );
		registry.ctx().emplace< UIConfig >();
	}

	return std::nullopt;
}
}  // namespace

App::App( const cli::Options& options )
    : m_options( options ),
      m_engine( { .Title = Title.data(),
                  .WindowWidth = WindowWidth,
                  .WindowHeight = WindowHeight,
                  .EnableGUI = options.gui } )
{}

auto App::init() -> std::optional< InitError >
{
	// TODO: Use path from CLI
	auto readResult = readPreset( m_options.presetPath );
	if ( !readResult )
	{
		return "-> Failed to load preset\n" + readResult.error();
	}

	auto& registry = m_engine.registry();
	if ( auto initError = initializeEntities( registry, m_options, readResult.value() ); initError )
	{
		return "-> Failed to initialize entities\n" + *initError;
	}

	initSystems();

	return std::nullopt;
}

auto App::run() -> void
{
	m_engine.run();
}

auto App::initSystems() -> void
{
	auto& registry = m_engine.registry();

	if ( m_options.gui )
	{
		assert( registry.ctx().contains< SFRenderService >() && "SFRenderService not initialized" );
		auto& renderer = registry.ctx().get< SFRenderService >();

		m_engine.addSystem< InputSystem >( registry );
		m_engine.addSystem< CameraMovementSystem >( registry );
		m_engine.addSystem< UISystem >( registry );
		m_engine.addSystem< RenderSystem >( registry, renderer );
	}
}
}  // namespace cc::app
