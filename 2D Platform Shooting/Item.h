#pragma once
#include "Level.h"

class Item {
private:
    bool isActive;

    sf::RectangleShape shape;
    sf::Vector2f velocity;

    float width;
    float height;
    bool OnAir;

    Level* level;

    uint32_t i_id;

public:
    Item(float x, float y, Level* level);

    void update(long long deltaTime);

    void draw(sf::RenderWindow& window);

    bool checkCollision(sf::FloatRect other);

    sf::Vector2f getPosition() const;

    bool checkCollisionBullet(sf::FloatRect other);

    uint32_t GetItemId() const { return i_id; }
};