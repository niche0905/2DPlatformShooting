#pragma once
#include "Level.h"

class Item {
private:
    bool isActive;

    uint32_t itemID;

    sf::RectangleShape shape;
    sf::Vector2f velocity;

    float width;
    float height;
    bool OnAir;

    Level* level;

public:
    Item(uint32_t i_id, float x, float y, Level* level);

    void update(long long deltaTime);

    void draw(sf::RenderWindow& window);

    bool checkCollision(sf::FloatRect other);

    sf::Vector2f getPosition() const;

    bool checkCollisionBullet(sf::FloatRect other);
};