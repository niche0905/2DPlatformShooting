#pragma once


#ifndef UTILITIES_H
#define UTILITIES_H

#include <SFML/System/Vector2.hpp>

sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);

#endif // UTILITIES_H