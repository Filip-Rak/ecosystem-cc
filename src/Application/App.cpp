#include "Application/App.hpp"

#include <cassert>
#include <cstdint>
#include <string_view>

#include <entt/fwd.hpp>

#include "Application/Constants/UIConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/SimRunnerData.hpp"
#include "Application/ContextEntity/UIConfig.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Application/System/CameraMovementSystem.hpp"
#include "Application/System/InputSystem.hpp"
#include "Application/System/RenderSystem.hpp"
#include "Application/System/Sim/SimRunnerSystem.hpp"
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
}  // namespace

App::App( const cli::Options& options )
    : m_CliOptions( options ),
      m_engine( { .Title = Title.data(),
                  .WindowWidth = WindowWidth,
                  .WindowHeight = WindowHeight,
                  .EnableGUI = options.gui } )
{}

auto App::init() -> std::optional< InitError >
{
	auto readResult = readPreset( m_CliOptions.presetPath );
	if ( !readResult )
	{
		return "-> Failed to read preset\n" + readResult.error();
	}

	auto& registry = m_engine.registry();
	if ( auto initError = initEntities( registry, readResult.value() ); initError )
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

auto App::initEntities( entt::registry& registry, const Preset& preset ) const -> std::optional< InitError >
{
	const auto readingError = readGridFromDirectory( registry, preset.gridDirectoryPath );
	if ( readingError )
	{
		return "-> Couldn't load the grid\n" + *readingError;
	}

	if ( m_CliOptions.gui )
	{
		auto& camera = registry.ctx().emplace< Camera >();
		constexpr const glm::vec2 CameraPosition{ constant::UI.SidePanel.InitialWidth / 2.f, 0.f };
		camera.position = glm::vec2{ CameraPosition };

		const auto& grid = registry.ctx().get< Grid >();
		registry.ctx().emplace< VisualGrid >( grid.cells.size() );
		registry.ctx().emplace< UIConfig >();
	}

	registry.ctx().emplace< Preset >( preset );
	registry.ctx().emplace< SimRunnerData >( SimRunnerData{ .paused = m_CliOptions.gui } );

	return std::nullopt;
}

auto App::initSystems() -> void
{
	auto& registry = m_engine.registry();
	if ( m_CliOptions.gui )
	{
		assert( registry.ctx().contains< SFRenderService >() && "SFRenderService not initialized" );
		auto& renderer = registry.ctx().get< SFRenderService >();

		m_engine.addSystem< InputSystem >( registry );
		m_engine.addSystem< CameraMovementSystem >( registry );
		m_engine.addSystem< SimRunnerSystem >( registry, m_CliOptions );
		m_engine.addSystem< UISystem >( registry );
		m_engine.addSystem< RenderSystem >( registry, renderer );
	}
	else
	{
		m_engine.addSystem< SimRunnerSystem >( registry, m_CliOptions );
	}
}
}  // namespace cc::app
