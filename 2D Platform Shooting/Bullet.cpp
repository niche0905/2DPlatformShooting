#include "Bullet.h"


bool Bullet::isOutBounds(float left, float right)
{
    float nowX = shape.getPosition().x;
    
    return nowX < left or right < nowX;
}

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
