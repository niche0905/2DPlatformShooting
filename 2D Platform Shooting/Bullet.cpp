#include "Bullet.h"


void Bullet::update(long long deltaTime)
{
    sf::Vector2f velocity(speed, 0.0f);

    if (direction) {
        shape.move(-velocity * (deltaTime / 1000000.0f));
    }
    else {
        shape.move(velocity * (deltaTime / 1000000.0f));
    }
}

void Bullet::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}
