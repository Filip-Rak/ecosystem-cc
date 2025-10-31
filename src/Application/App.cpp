#include "Application/App.hpp"

#include <cstdint>
#include <string_view>

#include <entt/fwd.hpp>

#include "Application/CLI/CLIOptions.hpp"
#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Application/System/CameraMovementSystem.hpp"
#include "Application/System/InputSystem.hpp"
#include "Application/System/RenderSystem.hpp"
#include "Application/System/UISystem.hpp"
#include "Engine/Service/SFRenderService.hpp"

namespace
{
constexpr uint16_t WindowWidth = 1280u;
constexpr uint16_t WindowHeight = 720u;
constexpr std::string_view Title = "Ecosystem";
}  // namespace

namespace cc::app
{
namespace
{
auto initializeGrid( entt::registry& registry, bool headless ) -> void
{
	// TODO: Load grid properties from file.
	constexpr uint16_t FixedDim = 100;  // WARN: Same as RenderSystem.
	auto& grid = registry.ctx().emplace< Grid >( FixedDim, FixedDim );

	const auto count = FixedDim * FixedDim;

	// TODO: Assume all properties range from 0.f to 1.f
	const float offset = 1.f / static_cast< float >( count );
	for ( std::size_t i = 0; i < count; i++ )
	{
		const float Value = static_cast< float >( i + 1 ) * offset;
		grid.cells.emplace_back( Value, Value, Value, Value );
	}

	if ( !headless ) registry.ctx().emplace< VisualGrid >( count );
}

auto initializeEntities( entt::registry& registry, bool headless ) -> void
{
	initializeGrid( registry, headless );

	if ( !headless ) registry.ctx().emplace< Camera >();
}
}  // namespace
App::App( const cli::Options& options )
    : m_engine( { .Title = Title.data(),
                  .WindowWidth = WindowWidth,
                  .WindowHeight = WindowHeight,
                  .EnableGUI = !options.headless } )
{
	auto& registry = m_engine.registry();
	initializeEntities( registry, options.headless );

	if ( !options.headless )
	{
		assert( registry.ctx().contains< SFRenderService >() && "SFRenderService not initialized" );

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
