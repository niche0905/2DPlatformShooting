#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Level.h"


class Player {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float width;
    float height;
    float speed;
    bool isJumping;
    float jumpHeight;
    sf::RenderWindow window;
    Level& level;

public:
    Player(float x, float y, Level& level) : width(50.0f), height(50.0f), speed(500.0f), isJumping(false), jumpHeight(750.0f), level(level) {
        shape.setOrigin(width / 2, height);
        shape.setSize(sf::Vector2f(50.0f, height));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
    }

    void update(long long dt); 

    void draw(sf::RenderWindow& window);

    sf::FloatRect getGlobalBounds() const;

    void handleCollision(sf::FloatRect other);
};