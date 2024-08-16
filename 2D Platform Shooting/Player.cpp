#include "Player.h"


void Player::update(long long deltaTime)
{
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
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
        velocity.y = -2.0f * 0.9810f * jumpHeight;
        isJumping = true;
    }

    shape.move(velocity * (deltaTime / 1000000.0f));

    for (const auto& platform : level.platforms) {
        if (CheckCollision(platform.getGlobalBounds())) {
            isJumping = false;
            velocity.y = 0.0f;
            shape.setPosition(shape.getPosition().x, platform.y);
        }
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Player::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

bool Player::CheckCollision(sf::FloatRect other) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    if (playerBounds.intersects(other)) {
        return true;
    }

    return false;
}