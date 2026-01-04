#include "Application/App.hpp"

#include <cassert>
#include <cstdint>
#include <string_view>

#include <entt/fwd.hpp>

#include "Application/Constants/UIConstants.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/Logger.hpp"
#include "Application/ContextEntity/Preset.hpp"
#include "Application/ContextEntity/Randomizer.hpp"
#include "Application/ContextEntity/SimLog.hpp"
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
constexpr uint16_t windowWidth   = 1280u;
constexpr uint16_t windowHeight  = 720u;
constexpr std::string_view title = "Ecosystem";
}  // namespace

App::App( const cli::Options& options )
    : m_cliOptions( options ),
      m_engine( { .title        = title.data(),
                  .windowWidth  = windowWidth,
                  .windowHeight = windowHeight,
                  .enableGUI    = options.gui } )
{}

auto App::init() -> std::optional< Error >
{
	const auto readResult = readPreset( m_cliOptions.presetPath );
	if ( !readResult )
	{
		return "-> Failed to read preset\n" + readResult.error();
	}

	auto& registry = m_engine.registry();
	if ( auto initError = initEntities( registry, readResult.value() ); initError )
	{
		return "-> Failed to initialize entities\n" + *initError;
	}

	assert( registry.ctx().contains< Preset >() );
	const auto preset = registry.ctx().get< Preset >();

	initSystems();

	return std::nullopt;
}

auto App::run() -> void
{
	m_engine.run();
}

auto App::initEntities( entt::registry& registry, const Preset& preset ) const -> std::optional< Error >
{
	registry.ctx().emplace< SimRunnerData >( SimRunnerData{ .paused = m_cliOptions.gui } );
	const auto& livePreset = registry.ctx().emplace< Preset >( preset );
	auto& logger           = registry.ctx().emplace< Logger >( registry );
	registry.ctx().emplace< Randomizer >( livePreset );
	registry.ctx().emplace< SimLog >();

	const auto gridArgs = readGridFromDirectory( registry, preset.gridDirectoryPath );
	if ( !gridArgs )
	{
		return "-> Couldn't load the grid\n" + gridArgs.error();
	}

	if ( auto error = logger.init( m_cliOptions.clean ); error )
	{
		return "-> Failed to initialize Logger\n" + *error;
	}

	Grid grid( gridArgs.value() );
	registry.ctx().emplace< Grid >( std::move( grid ) );

	if ( m_cliOptions.gui )
	{
		auto& camera = registry.ctx().emplace< Camera >();
		constexpr const glm::vec2 CameraPosition{ constant::UI.SidePanel.InitialWidth / 2.f, 0.f };
		camera.position = glm::vec2{ CameraPosition };

		registry.ctx().emplace< VisualGrid >( gridArgs->width, gridArgs->height );
		registry.ctx().emplace< UIConfig >();
	}

	return std::nullopt;
}

auto App::initSystems() -> void
{
	auto& registry = m_engine.registry();
	if ( m_cliOptions.gui )
	{
		assert( registry.ctx().contains< SFRenderService >() );
		auto& renderer = registry.ctx().get< SFRenderService >();

		m_engine.addSystem< InputSystem >( registry );
		m_engine.addSystem< CameraMovementSystem >( registry );
		m_engine.addSystem< SimRunnerSystem >( registry, m_cliOptions );
		m_engine.addSystem< UISystem >( registry );
		m_engine.addSystem< RenderSystem >( registry, renderer );
	}
	else
	{
		m_engine.addSystem< SimRunnerSystem >( registry, m_cliOptions );
	}
}
}  // namespace cc::app
