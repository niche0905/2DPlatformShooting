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
    // 피봇은 가운대 아래
    shape.setOrigin(width / 2, height);
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Green);

    // 총의 발사속도 제한을 위한 변수 초기화
    lastFireTime = std::chrono::system_clock::now();

    // 초기화 되지 않은 변수들 설정
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
    // 땅에 붙어있다면 마찰력이 작동하도록
    float frictionScale(1.0f);
    if (OnAir)
        frictionScale = 2.0f;

    // 0에 가까워지도록
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

    // 공중에 떠있는지 (점프 또는 플랫폼과 충돌 처리 후 정해짐)
    if (OnAir) {
        velocity.y += GravityAcc * GravityMul * (deltaTime / 1000000.0f);
    }

    damageControll(deltaTime);

    // 입은 피해량 만큼 넉백하게
    sf::Vector2f powerOfDamage(damaged, 0.0f);
    // 속도만큼 움직임
    shape.move((velocity + powerOfDamage) * (deltaTime / 1000000.0f));

    // 밟고있는 플랫폼이 없는지 플래그
    bool noOnePlatformCollide = true;

    if (not sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
        // 아래키가 눌리고 있지 않다면
        for (const auto& platform : level->platforms) {
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

    // 임시
    if (shape.getPosition().y > 1000.0f)    // 1000.0f 밑이라면 죽은 판정(임시임!)
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

void Dummy::reviveDummy()
{
    // 부활 시 처리해 할 행동들 추가하기
    isActive = true;    // 활성화 시키기

    // 맵 중앙 공중에 스폰
    shape.setPosition((level->leftBound + level->rightBound) / 2.0f, -1000.0f);  // -1000.0f 는 수정해야 할수도

    damaged = 0;
}

void Dummy::takeDamage(bool direction, float damage)
{
    if (direction == true)  // Left 면
        damaged -= damage * DamagePower;
    else                    // Right 면
        damaged += damage * DamagePower;
}