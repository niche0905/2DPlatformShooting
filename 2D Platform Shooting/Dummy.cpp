#include "pch.h"
#include "Dummy.h"
#include "Level.h"
#include "Gun.h"
#include "Bullet.h"
#include "Utilities.h"

Dummy::Dummy(float x, float y, Level* level, int texture_id)
    : isActive(true)
    , direction(true)
    , width(100.0f)
    , height(100.0f)
    , speed(500.0f)
    , jumpHeight(650.0f)
    , maxJumpChance(2)
    , jumpChance(maxJumpChance)
    , OnAir(false)
    , level(level)
    , image(texture_id)
    , life(3)
{
    // �Ǻ��� ����� �Ʒ�
    shape.setOrigin(width / 2, height);
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Green);

    // ���� �߻�ӵ� ������ ���� ���� �ʱ�ȭ
    lastFireTime = std::chrono::system_clock::now();

    // �ʱ�ȭ ���� ���� ������ ����
    curMag = -1;
    damaged = 0;
    gunId = 0;
    jumpChance = maxJumpChance;
}

void Dummy::fireBullet()
{
    if (curMag > 0) {
        if (--curMag == 0) {
            gunId = 0;
            curMag = -1;
        }
    }

    lastFireTime = std::chrono::system_clock::now();

    sf::Vector2f position = shape.getPosition();
    position.y -= 25.0f;

    bullets.push_back(Bullet(direction, position, GunInfo.gun_table[gunId].speed, GunInfo.gun_table[gunId].damage));
}

void Dummy::damageControll(long long deltaTime)
{
    // ���� �پ��ִٸ� �������� �۵��ϵ���
    float frictionScale(1.0f);
    if (OnAir)
        frictionScale = 2.0f;

    // 0�� �����������
    if (damaged > 0.0f) {
        damaged -= DamageScalingRatio * frictionScale * (deltaTime / 1000000.0f);
        damaged = std::max(0.0f, damaged);
    }
    else {
        damaged += DamageScalingRatio * frictionScale * (deltaTime / 1000000.0f);
        damaged = std::min(0.0f, damaged);
    }
}

void Dummy::setPosition(float x, float y)
{
    sf::Vector2f vec;
    vec.x = x;
    vec.y = y;

    shape.setPosition(vec);
}

void Dummy::update(long long deltaTime)
{
    updateBullets(deltaTime);

    if (not isActive) return;

    // ���߿� ���ִ��� (���� �Ǵ� �÷����� �浹 ó�� �� ������)
    if (OnAir) {
        velocity.y += GravityAcc * GravityMul * (deltaTime / 1000000.0f);
    }

    damageControll(deltaTime);

    // ���� ���ط� ��ŭ �˹��ϰ�
    sf::Vector2f powerOfDamage(damaged, 0.0f);
    // �ӵ���ŭ ������
    shape.move((velocity + powerOfDamage) * (deltaTime / 1000000.0f));

    // ����ִ� �÷����� ������ �÷���
    bool noOnePlatformCollide = true;

    if (not sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
        // �Ʒ�Ű�� ������ ���� �ʴٸ�
        for (const auto& platform : level->platforms) {
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

    // �ӽ�
    if (shape.getPosition().y > 1000.0f)    // 1000.0f ���̶�� ���� ����(�ӽ���!)
    {
        isActive = false;
        reviveDummy();
    }

    auto& pos = shape.getPosition();
    image.SetPosition(pos.x, pos.y + 25.f);
    image.scale(width * 2, height * 2);
    image.SetReversed(direction);
}

void Dummy::updateBullets(long long deltaTime)
{
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->update(deltaTime);
        if (it->isOutBounds(level->leftBound - 1000.0f, level->rightBound + 1000.0f))
            it = bullets.erase(it);
        else
            ++it;
    }
}

void Dummy::draw(sf::RenderWindow& window)
{
    if (not isActive) return;
    for (const Bullet& bullet : bullets) {
        bullet.draw(window);
    }

    image.draw(window);
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

void Dummy::reviveDummy()
{
    // ��Ȱ �� ó���� �� �ൿ�� �߰��ϱ�
    isActive = true;    // Ȱ��ȭ ��Ű��

    // �� �߾� ���߿� ����
    shape.setPosition((level->leftBound + level->rightBound) / 2.0f, -1000.0f);  // -1000.0f �� �����ؾ� �Ҽ���

    damaged = 0;
}

void Dummy::takeDamage(bool direction, float damage)
{
    if (direction == true)  // Left ��
        damaged -= damage * DamagePower;
    else                    // Right ��
        damaged += damage * DamagePower;
}