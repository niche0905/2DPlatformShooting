#include "Utilities.h"


sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
{
    return start + t * (end - start);
}
