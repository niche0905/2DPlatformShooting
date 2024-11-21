#pragma once
#include "Bullet.h"
#include "Level.h"
#include "Image.h"

constexpr float DamageScalingRatio = 50.0f;     // ���ط� ����ȭ ���(�������� ����)
constexpr float DamagePower = 5.0f;             // ������ ���� �� ��ġ ������
constexpr float FrictionScale = 5.0f;           // �������� ���� ��ų �� ������
constexpr float AirFrictionScale = 3.0f;        // �������� ���� ��ų �� ���� ���׷�
// �۽�ȣ �ڸ�Ʈ 09/26
// ���� ���� ���������� ����
// ���������� �������� �����ϴ°� �ƴ� �ٸ� ��ĵ� Ž���غ� �ʿ� ����


class Player {
private:
    bool isActive;      // �÷��̾� Ȱ��ȭ ��Ȱ��ȭ�� ��Ÿ���� ����

    sf::RectangleShape shape;
    sf::Vector2f velocity;

    bool direction;     // true : Left(��),	false : Right(��)
    float width;
    float height;
    float speed;

    float damaged;      // ���� ���� �� -(����) �������� ���� ���� +(���) ���������� ���� ����

    float jumpHeight;
    int jumpChance;     // ���� ��ȸ
    int maxJumpChance;  // �ִ� ���� ��ȸ (�ʱ�ȭ �� �� ���)
    bool OnAir;
    int life;

    // Ű�Է� �и�
    sf::Keyboard::Key upKeyBind;
    sf::Keyboard::Key downKeyBind;
    sf::Keyboard::Key leftKeyBind;
    sf::Keyboard::Key rightKeyBind;
    sf::Keyboard::Key attackKeyBind;

    // [cham] 9.22
    uint8_t gunId;
    
    std::list<Bullet> bullets;
    int curMag;
    std::chrono::system_clock::time_point lastFireTime;     // ���� �߻��� ������ �ð�

    bool fireKeyDown;
    bool leftKeyDown;
    bool rightKeyDown;

    Level* level;
    Image image{ TextureID::BANANA };

public:
    Player() = default;

    // ������ ��ġ���ް� level ������ �̿��� �浹üũ�� �ϱ����� �����Ѵ�
    Player(float x, float y, Level* level, int texture_id);

    // ������ ��ġ�� level ������ �ް� Key ���ε��� ���� ������ �Է¹޴´�
    Player(float x, float y, Level* level, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key attackKey, int texture_id);

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

    void setPosition(float x, float y);

    void getItem();

    // Getter
    int getCurMag() const { return curMag; }
    int getGunID() const { return gunId; }
    int getLife() const { return life; }

};


// Dummy ��ü ����(�۽�ȣ 09/03)
class Dummy {
    bool isActive;

    sf::RectangleShape shape;
    sf::Vector2f velocity;

    float damaged;      // ���� ���� �� -(����) �������� ���� ���� +(���) ���������� ���� ����

    bool direction;     // true : Left(��),	false : Right(��)
    float width;
    float height;
    float speed;

    float jumpHeight;
    int jumpChance;     // ���� ��ȸ
    int maxJumpChance;  // �ִ� ���� ��ȸ (�ʱ�ȭ �� �� ���)
    bool OnAir;
    int life;

    uint8_t gunId;

    std::list<Bullet> bullets;
    int curMag;
    std::chrono::system_clock::time_point lastFireTime;

    sf::RenderWindow window;
    Level* level;
    Image image{ TextureID::BANANA };

public:
    Dummy() = default;

    Dummy(float x, float y, Level* level, int texture_id);

    // ������ Input�� ó���Ѵ�
    void handleInput(const sf::Event& event);

    // ���̰� ���� �߻��Ѵ�.
    void fireBullet();

    // ������ ��ġ�� ������Ʈ�Ѵ�
    void update(long long deltaTime);

    // ���̰� �߻��� �Ѿ� ������Ʈ
    void updateBullets(long long deltaTime);

    // ���̸� �׸���
    void draw(sf::RenderWindow& window);

    // ���忡���� �ٿ���ڽ��� ��´�
    sf::FloatRect getGlobalBounds() const;

    // �浹üũ�� �Ѵ�
    bool checkCollision(sf::FloatRect other);

    // ������ ��ġ(�Ǻ� ��)�� ã�� �Լ�
    sf::Vector2f getPosition() const;

    // �Ѿ� �浹üũ(������ �浹üũ�� �ٸ��� �浹 ������ ó��)
    bool checkCollisionBullet(sf::FloatRect other);

    // ���̿��� �������� �ش�
    void takeDamage(bool direction, float damage);

    // ���ط��� �������ش�(������Ʈ)
    void damageControll(long long deltaTime);

    void setPosition(float x, float y);

    void getItem();

    // Getter
    int getCurMag() const { return curMag; }
    int getGunID() const { return gunId; }
    int getLife() const { return life; }

};