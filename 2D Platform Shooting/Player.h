#pragma once
#include "Bullet.h"
#include "Level.h"
#include "Image.h"

constexpr float DamageScalingRatio = 50.0f;     // ���ط� ����ȭ ���(�������� ����)
constexpr float DamagePower = 5.0f;             // ������ ���� �� ��ġ ������
constexpr float FrictionScale = 5.0f;           // �������� ���� ��ų �� ������
constexpr float AirFrictionScale = 3.0f;        // �������� ���� ��ų �� ���� ���׷�


class Player {
private:
    int32_t playerID{ -1 };

    // �÷��̾� Ȱ��ȭ ��Ȱ��ȭ�� ��Ÿ���� ����
    bool isActive;

    sf::RectangleShape shape;
    sf::Vector2f velocity;

    // true : Left(��),	false : Right(��)
    bool direction;
    float width;
    float height;
    float speed;

    // ���� ���� �� -(����) �������� ���� ���� +(���) ���������� ���� ����
    float damaged;

    float jumpHeight;
    // ���� ��ȸ
    int jumpChance;
    // �ִ� ���� ��ȸ (�ʱ�ȭ �� �� ���)
    int maxJumpChance;
    bool OnAir;
    int life;

    // Ű�Է� �и�
    sf::Keyboard::Key upKeyBind;
    sf::Keyboard::Key downKeyBind;
    sf::Keyboard::Key leftKeyBind;
    sf::Keyboard::Key rightKeyBind;
    sf::Keyboard::Key attackKeyBind;

    uint8_t gunId;
    
    std::list<Bullet> bullets;
    int curMag;
    // ���� �߻��� ������ �ð�
    std::chrono::system_clock::time_point lastFireTime;

    bool fireKeyDown;
    bool leftKeyDown;
    bool rightKeyDown;

    Level* level;
    Image image{ TextureID::BANANA };

    uint64_t bulletId;

public:
    Player() = default;

    // ������ ��ġ���ް� level ������ �̿��� �浹üũ�� �ϱ����� �����Ѵ�
    Player(float x, float y, Level* level, int texture_id, int32_t p_id);

    // ������ ��ġ�� level ������ �ް� Key ���ε��� ���� ������ �Է¹޴´�
    Player(float x, float y, Level* level, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key attackKey, int texture_id, int32_t p_id);

    bool isControl() const;

    // �÷��̾��� Input�� ó���Ѵ�
    void handleInput(const sf::Event& event);

    // �÷��̾ ���� �߻��Ѵ�.
    void fireBullet();

    // �÷��̾��� ��ġ�� ������Ʈ�Ѵ�
    void update(long long deltaTime); 

    // �÷��̾ �߻��� �Ѿ˵��� ������Ʈ ���ش�
    void updateBullets(long long deltaTime);

    // �÷��̾ �׸���
    void draw(sf::RenderWindow& window);

    // ���忡���� �ٿ���ڽ��� ��´�
    sf::FloatRect getGlobalBounds() const;

    // �浹üũ�� �Ѵ�
    bool checkCollision(sf::FloatRect other);

    // �÷��̾� ��ġ(�Ǻ� ��)�� ã�� �Լ�
    sf::Vector2f getPosition() const;

    // ��Ȱ�ϴ� �Լ�(Ȱ��ȭ)
    void revivePlayer();

    // �浹ó�� (������ �÷��� �浹 ó���� �ٸ�)
    bool checkCollisionBullet(sf::FloatRect other);

    // �÷��̾�� �������� ����
    void takeDamage(bool direction, float damage);

    // ���ط��� �������ش�(������Ʈ���� ȣ��)
    void damageControll(long long deltaTime);

    void removeBullet(uint32_t b_id);

    void decreaseMag();

    // Setter
    void setPosition(float x, float y);
    void SetDirection(const bool value) { direction = value; }
    void setPlayerGun(uint32_t g_id);

    // Getter
    int getCurMag() const { return curMag; }
    int getGunID() const { return gunId; }
    int getLife() const { return life; }
    int getDirection() const { return direction; }
};