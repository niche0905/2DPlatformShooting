#include "Player.h"


Player::Player(float x, float y, Level& level) : isActive(true), direction(true), width(50.0f), height(50.0f), speed(500.0f), jumpHeight(650.0f), maxJumpChance(2), jumpChance(maxJumpChance), OnAir(false), level(level), leftKeyDown(false), rightKeyDown(false)
{
    // 피봇은 가운대 아래
    shape.setOrigin(width / 2, height);
    shape.setSize(sf::Vector2f(50.0f, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Green);

    // 총의 발사속도 제한을 위한 변수 초기화
    lastFireTime = std::chrono::system_clock::now();

    // 키 입력을 받지 않았다면 기본값 설정
    upKeyBind = sf::Keyboard::Up;
    downKeyBind = sf::Keyboard::Down;
    leftKeyBind = sf::Keyboard::Left;
    rightKeyBind = sf::Keyboard::Right;
    attackKeyBind = sf::Keyboard::A;

    // 초기화 되지 않은 변수들 설정
    damaged = 0;
    gunId = 0;
    jumpChance = maxJumpChance;
}

Player::Player(float x, float y, Level& level, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key attackKey) : isActive(true), direction(true), width(50.0f), height(50.0f), speed(500.0f), jumpHeight(650.0f), maxJumpChance(2), jumpChance(maxJumpChance), OnAir(false), level(level), leftKeyDown(false), rightKeyDown(false)
{
    // 피봇은 가운대 아래
    shape.setOrigin(width / 2, height);
    shape.setSize(sf::Vector2f(50.0f, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Green);

    // 총의 발사속도 제한을 위한 변수 초기화
    lastFireTime = std::chrono::system_clock::now();

    // 입력받은 키로 키 바인딩
    upKeyBind = upKey;
    downKeyBind = downKey;
    leftKeyBind = leftKey;
    rightKeyBind = rightKey;
    attackKeyBind = attackKey;

    // 초기화 되지 않은 변수들 설정
    damaged = 0;
    gunId = 0;
    jumpChance = maxJumpChance;
}

void Player::handleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == upKeyBind) {
            // Space 키가 눌렸을 때 한 번만 실행되는 코드
            if (jumpChance > 0) {
                //velocity.y -= jumpHeight; // 이 둘 중 선택해야 함
                velocity.y = -jumpHeight;
                OnAir = true;
                --jumpChance;
            }
        }
        if (event.key.code == attackKeyBind) {
            auto nowTime = std::chrono::system_clock::now();
            // RPM에 따라 발사속도 제한 600이 Gun의 RPM이어야 함
            std::chrono::milliseconds deltaTime(int((60.0 / 600) * 1000));
            if ((std::chrono::duration_cast<std::chrono::milliseconds>(nowTime-lastFireTime)).count() >= deltaTime.count())
                fireBullet();
        }
        if (event.key.code == sf::Keyboard::D) {
            dash();
        }
        if (event.key.code == sf::Keyboard::Q) {
            gunId = getRandomGun();
        }
        if (event.key.code == sf::Keyboard::W) {
            gunId = 1;
        }
        if (event.key.code == sf::Keyboard::R) {
            revivePlayer();     // 임시로 키 바인딩으로 부활 호출
        }
    }
}

void Player::fireBullet()
{
    // 추후에 연사 속도나 남은 잔탄 수 같은 기능 넣어야 함
    lastFireTime = std::chrono::system_clock::now();

    sf::Vector2f position = shape.getPosition();
    position.y -= 25.0f;

    // TODO: 이 구조를 바꾸어야 함.
    // Bullet 객체를 생성할 때 총 포인터로만 생성할 수 있도록 제작
    // 

    // 임시 총 발싸
    bullets.push_back(Bullet(direction, position, g_guns[gunId].getSpeed()));
}

void Player::update(long long deltaTime)
{
    updateBullets(deltaTime);   // 비활성화 더라도 총알은 움직여야 하기에 위치 조정

    if (not isActive) return;   // 활성화 상태가 아니라면 Update 종료

    // 좌우 키가 눌리고 있는지
    leftKeyDown = sf::Keyboard::isKeyPressed(leftKeyBind);
    rightKeyDown = sf::Keyboard::isKeyPressed(rightKeyBind);

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

    damageControll(deltaTime);

    // 입은 피해량 만큼 넉백하게
    sf::Vector2f powerOfDamage(damaged, 0.0f);
    // 속도만큼 움직임
    shape.move((velocity + powerOfDamage) * (deltaTime / 1000000.0f));

    bool noOnePlatformCollide = true;

    // 하나라도 밟고있는 플랫폼이 있는지 체크
    if (not sf::Keyboard::isKeyPressed(downKeyBind)) {
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

    // 플랫폼에 붙어있지 않다면 공중 판정
    if (noOnePlatformCollide) {
        OnAir = true;
    }

    if (shape.getPosition().y > 1000.0f)    // 1000.0f 밑이라면 죽은 판정(임시임!)
    {
        isActive = false;
    }
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
    if (not isActive) return;   // 활성화 상태가 아니라면 Draw 종료

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

void Player::hitTheEnemy(class Player& otherPlayer)
{
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        // 맞았다면(충돌이라면)
        if (otherPlayer.checkCollisionBullet(it->getGlobalBounds())) {
            // 데미지를 적용하고
            otherPlayer.takeDamage(it->getDirection(), it->getDamage());

            // 총알 삭제
            it = bullets.erase(it);
        }
        else
            ++it;
    }
}

void Player::revivePlayer()
{
    if (isActive) return;   // 살아 있다면 revive 취소

    // 부활 시 처리해 할 행동들 추가하기
    isActive = true;    // 활성화 시키기

    // 맵 중앙 공중에 스폰
    shape.setPosition((level.leftBound+level.rightBound) / 2.0, -1000.0f);  // -1000.0f 는 수정해야 할수도
}

bool Player::checkCollisionBullet(sf::FloatRect other)
{
    sf::FloatRect playerBounds = shape.getGlobalBounds();
    if (playerBounds.intersects(other)) {
        return true;
    }

    return false;
}

void Player::takeDamage(bool direction, float damage)
{
    if (direction == true)  // Left 면
        damaged -= damage;
    else                    // Right 면
        damaged += damage;
}

void Player::damageControll(long long deltaTime)
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

    damageControll(deltaTime);

    // 입은 피해량 만큼 넉백하게
    sf::Vector2f powerOfDamage(damaged, 0.0f);
    // 속도만큼 움직임
    shape.move((velocity + powerOfDamage) * (deltaTime / 1000000.0f));

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
    if (direction == true)  // Left 면
        damaged -= damage;
    else                    // Right 면
        damaged += damage;
}
