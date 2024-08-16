#include "Player.h"


void Player::handleInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }

    bool LeftKey;
    bool RightKey;
    // 키입력에 따른 좌우 velocity 값 조정
    if (LeftKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -speed;
    }
    if (RightKey =sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = speed;
    }
    if (not (LeftKey or RightKey)) {
        velocity.x = 0.0f;
    }

    // spacebar키를 눌렀을 때 점프
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && not OnAir) {
        velocity.y = -2.0f * 0.9810f * jumpHeight;
        OnAir = true;
    }
}

void Player::update(long long deltaTime)
{
    handleInput();

    if (OnAir) {
        velocity.y += 0.9810f;
    }

    shape.move(velocity * (deltaTime / 1000000.0f));

    for (const auto& platform : level.platforms) {
        if (CheckCollision(platform.getGlobalBounds())) {
            OnAir = false;
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