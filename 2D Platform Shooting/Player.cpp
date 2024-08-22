#include "Player.h"


void Player::handleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            // Space 키가 눌렸을 때 한 번만 실행되는 코드
            if (jumpChance > 0) {
                //velocity.y -= jumpHeight; // 이 둘 중 선택해야 함
                velocity.y = -jumpHeight;
                OnAir = true;
                --jumpChance;
            }
        }
        if (event.key.code == sf::Keyboard::A) {
            fireBullet();
        }
    }
}

void Player::fireBullet()
{
    // 추후에 연사 속도나 남은 잔탄 수 같은 기능 넣어야 함

    sf::Vector2f position = shape.getPosition();
    position.y -= 25.0f;

    bullets.push_back(Bullet(direction, position));
}

void Player::update(long long deltaTime)
{
    // 좌우 키가 눌리고 있는지
    leftKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    rightKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    if (not (leftKeyDown and rightKeyDown)) {
        if (leftKeyDown) {
            direction = true;
            if (-speed <= velocity.x and velocity.x <= speed)
                velocity.x = -speed;
            else if (velocity.x > speed)
                velocity.x -= speed * (deltaTime / 1000000.0f);

        }
        else if (rightKeyDown) {
            direction = false;
            if (-speed <= velocity.x and velocity.x <= speed)
                velocity.x = speed;
            else if (velocity.x < -speed)
                velocity.x += speed * (deltaTime / 1000000.0f);
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

    updateBullets(deltaTime);
}

void Player::updateBullets(long long deltaTime)
{
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->update(deltaTime);
        if (it->isOutBounds(level.leftBound - 1000.0f, level.rightBound + 1000.0f))
            it = bullets.erase(it);
        else
            ++it;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);

    for (const Bullet& bullet : bullets) {
        bullet.draw(window);
    }
}

sf::FloatRect Player::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

bool Player::checkCollision(sf::FloatRect other) {
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    if (playerBounds.intersects(other) and velocity.y > 0) {
        float playerBottom = playerBounds.top + playerBounds.height;
        float platformTop = other.top;

        // 플레이어가 플랫폼의 위쪽에서 충돌했을 때만 true 반환
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
