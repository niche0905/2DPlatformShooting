#include "Player.h"


void Player::handleInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }

    bool LeftKey;
    bool RightKey;
    // Ű�Է¿� ���� �¿� velocity �� ����
    if (LeftKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -speed;
    }
    if (RightKey =sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = speed;
    }
    if (not (LeftKey or RightKey)) {
        velocity.x = 0.0f;
    }

    // spacebarŰ�� ������ �� ����
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

    bool noOnePlatformCollide = true;
    for (const auto& platform : level.platforms) {
        if (checkCollision(platform.getGlobalBounds())) {
            OnAir = false;
            velocity.y = 0.0f;
            shape.setPosition(shape.getPosition().x, platform.y);

            noOnePlatformCollide = false;
        }
    }
    if (noOnePlatformCollide) {
        OnAir = true;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Player::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

bool Player::checkCollision(sf::FloatRect other) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    if (playerBounds.intersects(other) and velocity.y > 0) {
        float playerBottom = playerBounds.top + playerBounds.height;
        float platformTop = other.top;

        // �÷��̾ �÷����� ���ʿ��� �浹���� ���� true ��ȯ
        if (playerBottom <= platformTop + 10) {
            return true;
        }
    }

    return false;
}