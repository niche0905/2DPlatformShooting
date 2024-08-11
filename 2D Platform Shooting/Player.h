#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>


class Player {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float speed;
    bool isJumping;
    float jumpHeight;
    sf::RenderWindow window;

public:
    Player(float x, float y) : speed(500.0f), isJumping(false), jumpHeight(750.0f) {
        shape.setSize(sf::Vector2f(50.0f, 50.0f));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
    }

    void update(long long dt); 

    void draw(sf::RenderWindow& window);

    sf::FloatRect getGlobalBounds() const;

    void handleCollision(sf::FloatRect other);
};