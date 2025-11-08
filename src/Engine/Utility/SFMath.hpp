#pragma once

#include <SFML/System/Vector2.hpp>
#include <glm/vec2.hpp>

namespace cc
{
template < typename T >
auto operator+( const glm::vec< 2, T >& g, const sf::Vector2< T >& s ) -> sf::Vector2< T >
{
	return { s.x + g.x, s.y + g.y };
}

template < typename T >
auto operator+( const sf::Vector2< T >& s, const glm::vec< 2, T >& g ) -> glm::vec< 2, T >
{
	return g + s;
}

template < typename T >
[[nodiscard]] auto toGlm( const sf::Vector2< T > sfVector )
{
	return glm::vec< 2, T >( sfVector.x, sfVector.y );
}

template < typename T >
[[nodiscard]] auto toSf( const glm::vec< 2, T > glmVector )
{
	return sf::Vector2< T >( glmVector.x, glmVector.y );
}
}  // namespace cc