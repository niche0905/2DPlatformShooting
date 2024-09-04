#include "Player.h"


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
    // ���Ŀ� ���� �ӵ��� ���� ��ź �� ���� ��� �־�� ��

    sf::Vector2f position = shape.getPosition();
    position.y -= 25.0f;

    gun.firebullet(direction, position);
}

void Player::update(long long deltaTime)
{
    // �¿� Ű�� ������ �ִ���
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

void Player::dash() {
    if (!direction) shape.move(100, 0);
    else shape.move(-100, 0);
}

void Player::hitTheEnemy(class Dummy& dummy)
{
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        if (dummy.checkCollisionBullet(it->getGlobalBounds())) {
        // �¾Ҵٸ�(�浹�̶��)
            // �������� �����ϰ�
            dummy.takeDamage(it->getDirection(), it->getDamage());

            // �Ѿ� ����
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

void Dummy::update(long long deltaTime)
{
    // �¿� Ű�� ������ �ִ���
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

    // ���߿� ���ִ��� (���� �Ǵ� �÷����� �浹 ó�� �� ������)
    if (OnAir) {
        velocity.y += GravityAcc * GravityMul * (deltaTime / 1000000.0f);
    }

    // �ӵ���ŭ ������
    shape.move(velocity * (deltaTime / 1000000.0f));

    // ����ִ� �÷����� ������ �÷���
    bool noOnePlatformCollide = true;

    if (not sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
    // �Ʒ�Ű�� ������ ���� �ʴٸ�
        for (const auto& platform : level.platforms) {
        // �÷��� ������ �浹ó��
            if (checkCollision(platform.getGlobalBounds())) {
            // �� �÷����̶� ������ ����ִ� �浹�̶��
                OnAir = false;
                jumpChance = maxJumpChance;
                velocity.y = 0.0f;
                shape.setPosition(shape.getPosition().x, platform.y);

                noOnePlatformCollide = false;
            }
        }
    }

    // ���߿� ���ִ� ���̴ٰ� ����
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

        // ���̰� �÷����� ���ʿ��� �浹���� ���� true ��ȯ
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
