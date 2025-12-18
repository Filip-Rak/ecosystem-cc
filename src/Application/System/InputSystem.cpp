#include "Application/System/InputSystem.hpp"

#include <cassert>
#include <print>  // FIXME: Debug

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

#include "Application/ContextEntity/Camera.hpp"
#include "Application/ContextEntity/Grid.hpp"
#include "Application/ContextEntity/VisualGrid.hpp"
#include "Engine/ContextEntity/Time.hpp"
#include "Engine/Events/SystemEvents.hpp"
#include "Engine/Service/GUIService.hpp"
#include "Engine/Service/InputService.hpp"

namespace cc::app
{
namespace
{
// TODO: Debug
auto updateDebug( entt::registry& registry, const InputService& input, const Time& time ) -> void
{
	if ( input.isPressed( keyboard::Key::Space ) )
	{
		std::print( "Run time: {}\nFPS: {}\nDeltaTime: {}\nMousePosition: {{ {}, {} }}\n", time.RunTime, time.FPS,
		            time.DeltaTime, input.getCurrentMousePos().x, input.getCurrentMousePos().y );
	}

	if ( input.isPressed( keyboard::Key::Escape ) )
	{
		auto& dispatcher = registry.ctx().get< entt::dispatcher >();
		dispatcher.enqueue< event::Exit >();
	}

	if ( input.isPressed( mouse::Button::Left ) )
	{
		auto& visualGrid = registry.ctx().get< VisualGrid >();
		auto& logicalGrid = registry.ctx().get< Grid >();

		if ( const auto cellPos = visualGrid.cellPositionUnderMouse; cellPos )
		{
			const auto cellIndex = ( cellPos->y * visualGrid.Height ) + cellPos->x;
			const auto cellPopulation = logicalGrid.spatialGrid[ cellIndex ].size();
			const Cell& cell = logicalGrid.cells[ cellIndex ];

			std::print( "Cell ID: {}\n-> Vegetation: {}\n-> Temperature: {}\n-> Humidity: {}\n-> Elevation: {}\n-> "
			            "Population: {}\n",
			            cellIndex, cell.vegetation, cell.temperature, cell.humidity, cell.elevation, cellPopulation );
		}
	}
}
}  // namespace
InputSystem::InputSystem( entt::registry& registry ) : m_registry( registry )
{
	assert( registry.ctx().contains< InputService >() );
	assert( registry.ctx().contains< GUIService >() );
	assert( registry.ctx().contains< VisualGrid >() );
	assert( registry.ctx().contains< Grid >() );
	assert( registry.ctx().contains< Time >() );
	assert( registry.ctx().contains< Camera >() );
}

auto InputSystem::update() -> void
{
	const auto& input = m_registry.ctx().get< InputService >();
	const auto& gui = m_registry.ctx().get< GUIService >();
	const auto& time = m_registry.ctx().get< Time >();
	auto& camera = m_registry.ctx().get< Camera >();

	using namespace keyboard;
	using namespace mouse;

	camera.keyboardMovementInput = { 0.f, 0.f };
	if ( input.isDown( Key::W ) ) camera.keyboardMovementInput.y -= 1;
	if ( input.isDown( Key::S ) ) camera.keyboardMovementInput.y += 1;
	if ( input.isDown( Key::A ) ) camera.keyboardMovementInput.x -= 1;
	if ( input.isDown( Key::D ) ) camera.keyboardMovementInput.x += 1;

	camera.keyboardZoomInput = 0.f;
	if ( input.isDown( Key::Up ) ) camera.keyboardZoomInput -= 1.f;
	if ( input.isDown( Key::Down ) ) camera.keyboardZoomInput += 1.f;

	if ( !gui.nowHandlesMouse() )
	{
		camera.mouseZoomInput = input.getMouseScrollDelta();
		camera.mouseMovementInput = ( input.isDown( Button::Left ) ) ? input.getMouseMoveDelta() : glm::ivec2{ 0 };
	}

	camera.isSpedUp = input.isDown( Key::LShift );
	updateDebug( m_registry, input, time );
}
}  // namespace cc::app