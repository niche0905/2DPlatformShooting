#include "Bullet.h"


void Bullet::update(long long deltaTime)
{
    sf::Vector2f velocity(speed, 0.0f);

    if (direction)  // 왼쪽으로 이동
    {
        shape.move(-velocity * (deltaTime / 1000000.0f));
    }
    else            // 오른쪽으로 이동
    {
        shape.move(velocity * (deltaTime / 1000000.0f));
    }
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}
