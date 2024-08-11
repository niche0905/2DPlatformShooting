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

    void update(long long dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x = -speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x = speed;
        }
        else {
            velocity.x = 0.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        if (isJumping) {
            velocity.y += 0.9810f;
            if (shape.getPosition().y >= 300.0f)
            {
                isJumping = false;
                velocity.y = 0.0f;
                shape.setPosition(shape.getPosition().x, 300.0f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
            velocity.y = -2.0f * 0.9810f * jumpHeight;
            isJumping = true;
        }

        shape.move(velocity * (dt / 1000000000.0f));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getGlobalBounds() const {
        return shape.getGlobalBounds();
    }

    void handleCollision(sf::FloatRect other) {
        sf::FloatRect playerBounds = shape.getGlobalBounds();
        if (playerBounds.intersects(other)) {
            isJumping = false;
        }
    }
};