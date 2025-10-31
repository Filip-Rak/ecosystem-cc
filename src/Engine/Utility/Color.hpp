#pragma once

#include <cstdint>

namespace cc
{
namespace
{
constexpr uint8_t Opaque{ 255 };
constexpr uint8_t BitDivisionBy256 = 8;
constexpr uint8_t Range{ 255 };
}  // namespace

struct Color
{
	uint8_t red{};
	uint8_t green{};
	uint8_t blue{};
	uint8_t alpha{ Opaque };
};

inline auto lerpColor( Color a, Color b, float t ) noexcept -> Color
{
	const auto scaledT = static_cast< uint8_t >( t * Range );
	auto lerp_channel = [ scaledT ]( uint8_t colorA, uint8_t colorB ) noexcept -> uint8_t
	{
		return static_cast< uint8_t >( colorA +
		                               ( ( ( colorB - colorA ) * scaledT ) >> BitDivisionBy256 ) );
	};

	return { .red = lerp_channel( a.red, b.red ),
	         .green = lerp_channel( a.green, b.green ),
	         .blue = lerp_channel( a.blue, b.blue ),
	         .alpha = lerp_channel( a.alpha, b.alpha ) };
}
}  // namespace cc