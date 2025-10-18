#include "Application/Application.hpp"

#include <cstdint>
#include <string_view>

namespace
{
constexpr uint16_t WindowWidth = 1280u;
constexpr uint16_t WindowHeight = 720u;
constexpr std::string_view Title = "Ecosystem";
}  // namespace

namespace cc::eco
{
Application::Application()
    : m_engine(
          { .Title = Title.data(), .WindowWidth = WindowWidth, .WindowHeight = WindowHeight } )
{}

auto Application::run() -> void
{
	m_engine.run();
}
}  // namespace cc::eco
