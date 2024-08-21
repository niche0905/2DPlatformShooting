#include "Player.h"
#include <iostream>


void Player::handleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            // Space Ű�� ������ �� �� ���� ����Ǵ� �ڵ�
            if (jumpChance > 0) {
                //velocity.y -= jumpHeight; // �� �� �� �����ؾ� ��
                velocity.y = -jumpHeight;
                OnAir = true;
                --jumpChance;
            }
        }
    }
}

void Player::update(long long deltaTime)
{
    // �¿� Ű�� ������ �ִ���
    leftKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    rightKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    if (not (leftKeyDown and rightKeyDown)) {
        if (leftKeyDown) {
            velocity.x = -speed;
        }
        else if (rightKeyDown) {
            velocity.x = speed;
        }
        else {
            velocity.x = 0.0f;
        }
    }

    if (OnAir) {
        velocity.y += GravityAcc * GravityMul * (deltaTime / 1000000.0f);
    }

    shape.move(velocity * (deltaTime / 1000000.0f));

    bool noOnePlatformCollide = true;

    //bool downKeyDown = );
    if (not sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        for (const auto& platform : level.platforms) {
            if (checkCollision(platform.getGlobalBounds())) {
                OnAir = false;
                jumpChance = maxJumpChance;
                velocity.y = 0.0f;
                shape.setPosition(shape.getPosition().x, platform.y);

                noOnePlatformCollide = false;
            }
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
        if (playerBottom <= platformTop + PlatformUp) {
            return true;
        }
    }

    return false;
}

sf::Vector2f Player::getPosition() const
{
    return shape.getPosition();
}
