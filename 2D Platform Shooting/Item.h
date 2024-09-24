#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
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

public:
    Item(float x, float y, Level* level);

    void update(long long deltaTime);

    void draw(sf::RenderWindow& window);

    bool checkCollision(sf::FloatRect other);

    sf::Vector2f getPosition() const;
};