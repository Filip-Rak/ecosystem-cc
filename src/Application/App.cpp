#include "Application/App.hpp"

#include <cstdint>
#include <string_view>

#include <entt/fwd.hpp>

#include "Application/CLI/CLIOptions.hpp"
#include "Application/Grid/Grid.hpp"
#include "Application/System/InputSystem.hpp"
#include "Application/System/UISystem.hpp"

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
auto initializeGrid( entt::registry& registry ) -> void
{
	// TODO: Load grid properties from file.
	constexpr int fixedDim = 100;
	auto& grid = registry.ctx().emplace< Grid >( fixedDim, fixedDim );
}
}  // namespace

App::App( const cli::Options& options )
    : m_engine( { .Title = Title.data(),
                  .WindowWidth = WindowWidth,
                  .WindowHeight = WindowHeight,
                  .EnableGUI = !options.headless } )
{
	auto& registry = m_engine.registry();
	initializeGrid( registry );

	if ( !options.headless )
	{
		m_engine.addSystem< InputSystem >( registry );
		m_engine.addSystem< UISystem >( registry );
	}
}

auto App::run() -> void
{
	m_engine.run();
}
}  // namespace cc::app
