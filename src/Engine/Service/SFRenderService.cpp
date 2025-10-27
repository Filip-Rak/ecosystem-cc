#include "Engine/Service/SFRenderService.hpp"

#include <iostream>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <entt/entt.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "Engine/Events/GUIEvents.hpp"

namespace cc
{
namespace
{
// TODO: FIgure out a better place for this -> should be part of the base.
auto rebuildFont() -> void
{
	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->Clear();

	const float UiScale = io.FontGlobalScale;
	constexpr float BaseFontSize = 13.f;

	ImFontConfig cfg;
	// cfg.SizePixels = BaseFontSize * UiScale;  // FIXME: Do something about this stupidity.

	io.Fonts->AddFontDefault( &cfg );
	io.Fonts->Build();

	if ( !ImGui::SFML::UpdateFontTexture() )
	{
		std::cerr << "SFRenderService::rebuildFont() -> Font update failed\n";
	}
}
}  // namespace

SFRenderService::SFRenderService( entt::registry& registry, sf::RenderWindow& window )
    : m_window( window )
{
	assert( registry.ctx().contains< entt::dispatcher >() && "Dispatcher not initialized" );
	auto& dispatcher = registry.ctx().get< entt::dispatcher >();
	dispatcher.sink< event::RebuildFont >().connect< &SFRenderService::onRebuildFont >( *this );
}

auto SFRenderService::beginFrame( entt::registry& /*registry*/ ) -> void
{
	m_window.clear();
}

auto SFRenderService::endFrame( entt::registry& /*registry*/ ) -> void
{
	ImGui::SFML::Render( m_window );
	m_window.display();

	if ( !m_updatedFont )
	{
		rebuildFont();
		m_updatedFont = true;
	}
}

auto SFRenderService::onRebuildFont( const event::RebuildFont& /*event*/ ) -> void
{
	m_updatedFont = false;
}
}  // namespace cc