#include "pch.h"
#include "Player.h"
#include "Level.h"
#include "Gun.h"
#include "Bullet.h"
#include "Utilities.h"
#include "Scene.h"

Player::Player(float x, float y, Level* level, int texture_id, int32_t p_id)
    : playerID(p_id)
    , isActive(true)
    , direction(true)
    , width(myNP::PlayerWidth)
    , height(myNP::PlayerHeight)
    , speed(500.0f)
    , jumpHeight(650.0f)
    , maxJumpChance(2)
    , jumpChance(maxJumpChance)
    , OnAir(false)
    , level(level)
    , leftKeyDown(false)
    , rightKeyDown(false)
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

    // 키 입력을 받지 않았다면 기본값 설정
    upKeyBind = sf::Keyboard::Up;
    downKeyBind = sf::Keyboard::Down;
    leftKeyBind = sf::Keyboard::Left;
    rightKeyBind = sf::Keyboard::Right;
    attackKeyBind = sf::Keyboard::A;

    // 초기화 되지 않은 변수들 설정
    curMag = -1;
    damaged = 0;
    gunId = 0;
    jumpChance = maxJumpChance;
}

Player::Player(float x, float y, Level* level, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key attackKey, int texture_id, int32_t p_id)
    : playerID(p_id)
    , isActive(true)
    , direction(true)
    , width(myNP::PlayerWidth)
    , height(myNP::PlayerHeight)
    , speed(500.0f)
    , jumpHeight(650.0f)
    , maxJumpChance(2)
    , jumpChance(maxJumpChance)
    , OnAir(false)
    , level(level)
    , leftKeyDown(false)
    , rightKeyDown(false)
    , image(texture_id)
    , life(3)
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
    curMag = -1;
    damaged = 0;
    gunId = 0;
    jumpChance = maxJumpChance;
}

bool Player::isControl() const
{
    return (network_mgr.GetClientID() == playerID);
}

void Player::handleInput(const sf::Event& event)
{
    if (not isControl())
        return;

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
        //if (event.key.code == attackKeyBind) {
        //    //auto nowTime = std::chrono::system_clock::now();
        //    //// RPM에 따라 발사속도 제한 600이 Gun의 RPM이어야 함 <- (수정함 09/23 송승호)
        //    //std::chrono::milliseconds deltaTime(int((60.0 / g_guns[gunId].RPM) * 1000));
        //    //if ((std::chrono::duration_cast<std::chrono::milliseconds>(nowTime-lastFireTime)).count() >= deltaTime.count())
        //    //    fireBullet();
        //}
        if (event.key.code == sf::Keyboard::Q) {
            gunId = GunLoader::Instance().getRandomGunId();
        }
        if (event.key.code == sf::Keyboard::W) {
            gunId = 1;
        }
    }
}

void Player::fireBullet()
{
    if (not isControl())
        return;

    decreaseMag();

    lastFireTime = std::chrono::system_clock::now();

    sf::Vector2f position = shape.getPosition();
    position.y -= 25.0f;

    // 내 bullet list에서도 id를 저장할 필요가 있음.
    Bullet new_bullet{ direction, position,
        GunLoader::Instance().GetGunTable()[gunId].speed,
        GunLoader::Instance().GetGunTable()[gunId].damage };
    new_bullet.SetBulletId(bulletId);
    bullets.push_back(new_bullet);


    // FirePacket 보내기
    auto buf = myNP::CS_FIRE_PACKET::MakePacket(bulletId, position.x, position.y, direction, gunId, timer.epochToMillis());
    network_mgr.SendPacket(
        reinterpret_cast<char*>(&buf),
        myNP::CS_FIRE
    );
}

void Player::update(long long deltaTime)
{
    auto& pos = shape.getPosition();
    image.SetPosition(pos.x, pos.y + 25.f);
    image.scale(width * 2, height * 2);
    image.SetReversed(direction);

    // 비활성화 더라도 총알은 움직여야 하기에 위치 조정
    updateBullets(deltaTime);

    if (not isControl())
        return;

    // 활성화 상태가 아니라면 Update 종료
    if (not isActive) return;

    // 좌우 키가 눌리고 있는지
    leftKeyDown = sf::Keyboard::isKeyPressed(leftKeyBind);
    rightKeyDown = sf::Keyboard::isKeyPressed(rightKeyBind);
    fireKeyDown = sf::Keyboard::isKeyPressed(attackKeyBind);
    
    bool focus = sceneManager.GetActiveScene()->GetFocus();

    if (focus and fireKeyDown) {
        auto nowTime = std::chrono::system_clock::now();
        std::chrono::milliseconds deltaTime(int((60.0 / GunLoader::Instance().GetGunTable()[gunId].RPM) * 1000));
        if ((std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - lastFireTime)).count() >= deltaTime.count())
            fireBullet();
    }

    if (focus) {
        if (not (leftKeyDown and rightKeyDown)) {
            if (leftKeyDown) {
                direction = true;
                if (-speed <= velocity.x and velocity.x <= speed)
                    velocity.x = -speed;
                else if (velocity.x > speed)
                    velocity.x -= speed * (deltaTime / myNP::microToSecond);

            }
            else if (rightKeyDown) {
                direction = false;
                if (-speed <= velocity.x and velocity.x <= speed)
                    velocity.x = speed;
                else if (velocity.x < -speed)
                    velocity.x += speed * (deltaTime / myNP::microToSecond);
            }
            else {
                velocity.x = 0.0f;
            }
        }
    }

    if (OnAir) {
        velocity.y += GravityAcc * GravityMul * (deltaTime / myNP::microToSecond);
    }

    // 피해량 조절
    damageControll(deltaTime);

    // 입은 피해량 만큼 넉백하게
    sf::Vector2f powerOfDamage(damaged, 0.0f);
    // 속도만큼 움직임
    shape.move((velocity + powerOfDamage) * (deltaTime / myNP::microToSecond));

    bool noOnePlatformCollide = true;

    bool downKeyDown = sf::Keyboard::isKeyPressed(downKeyBind);
        // 하나라도 밟고있는 플랫폼이 있는지 체크
    if (not (focus and downKeyDown)) {
        for (const auto& platform : level->platforms) {
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

    // Timer Update
    if (timer.isSyncTime()) {
        auto buf = myNP::CS_MOVE_PACKET::MakePacket(playerID, pos.x, pos.y, direction);
        network_mgr.SendPacket(
            reinterpret_cast<char*>(&buf),
            myNP::CS_MOVE
        );

        WaitForSingleObject(network_mgr.GetProcessEvent(), WSA_INFINITE);

        network_mgr.ProcessPacket();

        SetEvent(network_mgr.GetRecvEvent());
    }

}

void Player::updateBullets(long long deltaTime)
{
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->update(deltaTime);
        if (it->isOutBounds(level->leftBound - 1000.0f, level->rightBound + 1000.0f))
            it = bullets.erase(it);
        else
            ++it;
    }
}

void Player::draw(sf::RenderWindow& window) {
    if (not isActive) return;

    for (const Bullet& bullet : bullets) {
        bullet.draw(window);
    }

    image.draw(window);
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

void Player::revivePlayer()
{
    if (playerID == network_mgr.GetClientID()) {
        isActive = true;

        // 맵 중앙 공중에 스폰
        shape.setPosition((level->leftBound + level->rightBound) / 2.0f, -1000.0f);  // -1000.0f 는 수정해야 할수도

        damaged = 0;
    }
    life -= 1;
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
        damaged -= damage * DamagePower;
    else                    // Right 면
        damaged += damage * DamagePower;
}

void Player::damageControll(long long deltaTime)
{
    // 땅에 붙어있다면 마찰력이 작동하도록
    float frictionScale(FrictionScale);
    if (OnAir)
        frictionScale = AirFrictionScale;

    // 0에 가까워지도록
    if (damaged > 0.0f) {
        damaged -= DamageScalingRatio * frictionScale * (deltaTime / myNP::microToSecond);
        damaged = std::max(0.0f, damaged);
    }
    else {
        damaged += DamageScalingRatio * frictionScale * (deltaTime / myNP::microToSecond);
        damaged = std::min(0.0f, damaged);
    }
}

void Player::removeBullet(uint32_t b_id)
{
    bullets.remove_if([b_id](const Bullet& bullet) {
        return bullet.GetBulletId() == b_id;
        });
}

void Player::decreaseMag()
{
    bulletId++;
    if (curMag > 0) {
        if (--curMag == 0) {
            gunId = 0;
            curMag = -1;
        }
    }
}

void Player::setPosition(float x, float y)
{
    sf::Vector2f vec;
    vec.x = x;
    vec.y = y;

    shape.setPosition(vec);
}

void Player::setPlayerGun(uint32_t g_id)
{
    gunId = g_id;
    curMag = GunLoader::Instance().GetGunTable()[gunId].mag;
}
