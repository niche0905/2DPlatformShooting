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
        if (event.key.code == sf::Keyboard::D) {
            dash();
        }
    }
}

void Player::fireBullet()
{
    // 추후에 연사 속도나 남은 잔탄 수 같은 기능 넣어야 함

    sf::Vector2f position = shape.getPosition();
    position.y -= 25.0f;

    gun.firebullet(direction, position);
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
    gun.updateBullets(deltaTime);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);

    gun.draw(window);
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

void Player::dash() {
    if (!direction) shape.move(100, 0);
    else shape.move(-100, 0);
}

void Player::hitTheEnemy(class Dummy& dummy)
{
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        if (dummy.checkCollisionBullet(it->getGlobalBounds())) {
        // 맞았다면(충돌이라면)
            // 데미지를 적용하고
            dummy.takeDamage(it->getDirection(), it->getDamage());

            // 총알 삭제
            it = bullets.erase(it);
        }
        else
            ++it;
    }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Dummy::handleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::I) {
            // Space 키가 눌렸을 때 한 번만 실행되는 코드
            if (jumpChance > 0) {
                //velocity.y -= jumpHeight; // 이 둘 중 선택해야 함
                velocity.y = -jumpHeight;
                OnAir = true;
                --jumpChance;
            }
        }
    }
}

void Dummy::update(long long deltaTime)
{
    // 좌우 키가 눌리고 있는지
    leftKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::J);
    rightKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::L);

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

    // 공중에 떠있는지 (점프 또는 플랫폼과 충돌 처리 후 정해짐)
    if (OnAir) {
        velocity.y += GravityAcc * GravityMul * (deltaTime / 1000000.0f);
    }

    // 속도만큼 움직임
    shape.move(velocity * (deltaTime / 1000000.0f));

    // 밟고있는 플랫폼이 없는지 플래그
    bool noOnePlatformCollide = true;

    if (not sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
    // 아래키가 눌리고 있지 않다면
        for (const auto& platform : level.platforms) {
        // 플랫폼 루프로 충돌처리
            if (checkCollision(platform.getGlobalBounds())) {
            // 한 플랫폼이라도 위에서 밟고있는 충돌이라면
                OnAir = false;
                jumpChance = maxJumpChance;
                velocity.y = 0.0f;
                shape.setPosition(shape.getPosition().x, platform.y);

                noOnePlatformCollide = false;
            }
        }
    }

    // 공중에 떠있는 것이다고 판정
    if (noOnePlatformCollide) {
        OnAir = true;
    }
}

void Dummy::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Dummy::getGlobalBounds() const
{
    return shape.getGlobalBounds();
}

bool Dummy::checkCollision(sf::FloatRect other)
{
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    if (playerBounds.intersects(other) and velocity.y > 0) {
        float playerBottom = playerBounds.top + playerBounds.height;
        float platformTop = other.top;

        // 더미가 플랫폼의 위쪽에서 충돌했을 때만 true 반환
        if (playerBottom <= platformTop + PlatformUp) {
            return true;
        }
    }

    return false;
}

sf::Vector2f Dummy::getPosition() const
{
    return shape.getPosition();
}

bool Dummy::checkCollisionBullet(sf::FloatRect other)
{
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    if (playerBounds.intersects(other)) {
        return true;
    }

    return false;
}

void Dummy::takeDamage(bool direction, float damage)
{

}
