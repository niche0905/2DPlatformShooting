#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <chrono>
#include "Level.h"
#include "Bullet.h"
#include "Gun.h"
#include "Utilities.h"

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

public:
    Player() = default;

    // ������ ��ġ���ް� level ������ �̿��� �浹üũ�� �ϱ����� �����Ѵ�
    Player(float x, float y, Level* level);

    // ������ ��ġ�� level ������ �ް� Key ���ε��� ���� ������ �Է¹޴´�
    Player(float x, float y, Level* level, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key attackKey);

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

    // �÷��̾� �뽬(�ΰ�� �׽�Ʈ)
    void dash();

    // �Ѿ˵��� ���� ������� �˻�
    void hitTheEnemy(class Dummy& dummy);

    // �Ѿ˵��� ���� ������� �˻�
    void hitTheEnemy(class Player& otherPlayer);

    // ��Ȱ�ϴ� �Լ�(Ȱ��ȭ)
    void revivePlayer();

    // �浹ó�� (������ �÷��� �浹 ó���� �ٸ�)
    bool checkCollisionBullet(sf::FloatRect other);

    // �÷��̾�� �������� ����
    void takeDamage(bool direction, float damage);

    // ���ط��� �������ش�(������Ʈ���� ȣ��)
    void damageControll(long long deltaTime);

    void getItem();
};


// Dummy ��ü ����(�۽�ȣ 09/03)
class Dummy {
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

    bool leftKeyDown;
    bool rightKeyDown;

    sf::RenderWindow window;
    Level& level;

public:
    // ������ ��ġ���ް� level ������ �̿��� �浹üũ�� �ϱ����� �����Ѵ�
    Dummy(float x, float y, Level& level) : direction(true), width(50.0f), height(50.0f), speed(500.0f), jumpHeight(650.0f), maxJumpChance(2), jumpChance(maxJumpChance), OnAir(false), level(level), leftKeyDown(false), rightKeyDown(false)
    {
        // �Ǻ��� ����� �Ʒ�
        shape.setOrigin(width / 2, height);
        shape.setSize(sf::Vector2f(50.0f, height));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Red);
    }

    // ������ Input�� ó���Ѵ�
    void handleInput(const sf::Event& event);

    // ���ط��� �������ش�(������Ʈ)
    void damageControll(long long deltaTime);

    // ������ ��ġ�� ������Ʈ�Ѵ�
    void update(long long deltaTime);

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
};