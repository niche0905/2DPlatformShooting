#include "pch.h"
#include "Player.h"
#include "Level.h"
#include "Gun.h"
#include "Bullet.h"
#include "Utilities.h"

Player::Player(float x, float y, Level* level, int texture_id, int player_id)
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
    , leftKeyDown(false)
    , rightKeyDown(false)
    , image(texture_id)
    , life(3)
    , playerId(player_id)
{
    // �Ǻ��� ����� �Ʒ�
    shape.setOrigin(width / 2, height);
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Green);

    // ���� �߻�ӵ� ������ ���� ���� �ʱ�ȭ
    lastFireTime = std::chrono::system_clock::now();

    // Ű �Է��� ���� �ʾҴٸ� �⺻�� ����
    upKeyBind = sf::Keyboard::Up;
    downKeyBind = sf::Keyboard::Down;
    leftKeyBind = sf::Keyboard::Left;
    rightKeyBind = sf::Keyboard::Right;
    attackKeyBind = sf::Keyboard::A;

    // �ʱ�ȭ ���� ���� ������ ����
    curMag = -1;
    damaged = 0;
    gunId = 0;
    jumpChance = maxJumpChance;
}

Player::Player(float x, float y, Level* level, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key attackKey, int texture_id)
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
    , leftKeyDown(false)
    , rightKeyDown(false)
    , image(texture_id)
    , life(3)
{
    // �Ǻ��� ����� �Ʒ�
    shape.setOrigin(width / 2, height);
    shape.setSize(sf::Vector2f(50.0f, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Green);

    // ���� �߻�ӵ� ������ ���� ���� �ʱ�ȭ
    lastFireTime = std::chrono::system_clock::now();

    // �Է¹��� Ű�� Ű ���ε�
    upKeyBind = upKey;
    downKeyBind = downKey;
    leftKeyBind = leftKey;
    rightKeyBind = rightKey;
    attackKeyBind = attackKey;

    // �ʱ�ȭ ���� ���� ������ ����
    curMag = -1;
    damaged = 0;
    gunId = 0;
    jumpChance = maxJumpChance;
}

void Player::handleInput(const sf::Event& event)
{
    if (playerId != network_mgr.GetClientId()) return;

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == upKeyBind) {
            // Space Ű�� ������ �� �� ���� ����Ǵ� �ڵ�
            if (jumpChance > 0) {
                //velocity.y -= jumpHeight; // �� �� �� �����ؾ� ��
                velocity.y = -jumpHeight;
                OnAir = true;
                --jumpChance;
            }
        }
        if (event.key.code == attackKeyBind) {
            //auto nowTime = std::chrono::system_clock::now();
            //// RPM�� ���� �߻�ӵ� ���� 600�� Gun�� RPM�̾�� �� <- (������ 09/23 �۽�ȣ)
            //std::chrono::milliseconds deltaTime(int((60.0 / g_guns[gunId].RPM) * 1000));
            //if ((std::chrono::duration_cast<std::chrono::milliseconds>(nowTime-lastFireTime)).count() >= deltaTime.count())
            //    fireBullet();
        }
        if (event.key.code == sf::Keyboard::Q) {
            gunId = GunLoader::Instance().getRandomGunId();
        }
        if (event.key.code == sf::Keyboard::W) {
            gunId = 1;
        }
        if (event.key.code == sf::Keyboard::R) {
            // revivePlayer();     // �ӽ÷� Ű ���ε����� ��Ȱ ȣ��
            auto buf = myNP::CS_MATCHMAKING_PACKET::MakePacket();
            network_mgr.SendPacket(
                reinterpret_cast<char*>(&buf),
                myNP::CS_MATCHMAKING
            );
        }
    }
}

void Player::fireBullet()
{
    bulletId++;
    if (curMag > 0) {
        if (--curMag == 0) {
            gunId = 0;
            curMag = -1;
        }
    }

    lastFireTime = std::chrono::system_clock::now();

    sf::Vector2f position = shape.getPosition();
    position.y -= 25.0f;

    bullets.push_back(Bullet(direction, position, GunLoader::Instance().GetGunTable()[gunId].speed, GunLoader::Instance().GetGunTable()[gunId].damage));

    // FirePacket ������
    /*auto buf = myNP::CS_FIRE_PACKET::MakePacket(bulletId, position.x, position.y, direction, gunId, timer.epochToMillis());
    network_mgr.SendPacket(
        reinterpret_cast<char*>(&buf),
        myNP::CS_FIRE
    );*/
}

void Player::update(long long deltaTime)
{
    timer.Update();

    updateBullets(deltaTime);   // ��Ȱ��ȭ ���� �Ѿ��� �������� �ϱ⿡ ��ġ ����

    if (not isActive) return;   // Ȱ��ȭ ���°� �ƴ϶�� Update ����

    // �¿� Ű�� ������ �ִ���

    if (playerId == network_mgr.GetClientId()) {

        leftKeyDown = sf::Keyboard::isKeyPressed(leftKeyBind);
        rightKeyDown = sf::Keyboard::isKeyPressed(rightKeyBind);
        fireKeyDown = sf::Keyboard::isKeyPressed(attackKeyBind);

        if (fireKeyDown) {
            auto nowTime = std::chrono::system_clock::now();
            std::chrono::milliseconds deltaTime(int((60.0 / GunLoader::Instance().GetGunTable()[gunId].RPM) * 1000));
            if ((std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - lastFireTime)).count() >= deltaTime.count())
                fireBullet();
        }

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

    }

    // ���� ���ط� ��ŭ �˹��ϰ�
    if (OnAir) {
        velocity.y += GravityAcc * GravityMul * (deltaTime / 1000000.0f);
    }

    damageControll(deltaTime);

    // ���� ���ط� ��ŭ �˹��ϰ�
    sf::Vector2f powerOfDamage(damaged, 0.0f);

    // �ӵ���ŭ ������
    shape.move((velocity + powerOfDamage) * (deltaTime / 1000000.0f));

    bool noOnePlatformCollide = true;

    // �ϳ��� ����ִ� �÷����� �ִ��� üũ
    if (not sf::Keyboard::isKeyPressed(downKeyBind) ||
        playerId != network_mgr.GetClientId()) {
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

    // �÷����� �پ����� �ʴٸ� ���� ����
    if (noOnePlatformCollide) {
        OnAir = true;
    }


    // �ӽ�
    if (shape.getPosition().y > 1000.0f)    // 1000.0f ���̶�� ���� ����(�ӽ���!)
    {
        isActive = false;
        revivePlayer();
    }

    auto& pos = shape.getPosition();
    image.SetPosition(pos.x, pos.y + 25.f);
    image.scale(width * 2, height * 2);
    image.SetReversed(direction);


    // Timer Update
    if (timer.isSyncTime()) {
        // send move packet.
        // �ӽ÷� 0��. �÷��̾� id �����ϱ�
        //auto buf = myNP::CS_MOVE_PACKET::MakePacket(0, pos.x, pos.y, direction);
        //network_mgr.SendPacket(
        //    reinterpret_cast<char*>(&buf),
        //    myNP::CS_MOVE
        //);

        // TODO: ���� ���϶��� ������
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

void Player::revivePlayer()
{
    // ��Ȱ �� ó���� �� �ൿ�� �߰��ϱ�
    isActive = true;    // Ȱ��ȭ ��Ű��

    // �� �߾� ���߿� ����
    shape.setPosition((level->leftBound+level->rightBound) / 2.0f, -1000.0f);  // -1000.0f �� �����ؾ� �Ҽ���

    damaged = 0;
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
    if (direction == true)  // Left ��
        damaged -= damage * DamagePower;
    else                    // Right ��
        damaged += damage * DamagePower;
}

void Player::damageControll(long long deltaTime)
{
    // ���� �پ��ִٸ� �������� �۵��ϵ���
    float frictionScale(FrictionScale);
    if (OnAir)
        frictionScale = AirFrictionScale;

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

void Player::setPosition(float x, float y)
{
    sf::Vector2f vec;
    vec.x = x;
    vec.y = y;

    shape.setPosition(vec);
}