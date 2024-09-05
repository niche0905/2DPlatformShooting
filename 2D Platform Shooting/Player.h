#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include "Level.h"
#include "Bullet.h"

constexpr float GravityAcc = 9.810f;
constexpr float GravityMul = 120.0f;
constexpr float PlatformUp = 10.0f;


class Player {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;

    bool direction;     // true : Left(��),	false : Right(��)
    float width;
    float height;
    float speed;

    float jumpHeight;
    int jumpChance;     // ���� ��ȸ
    int maxJumpChance;  // �ִ� ���� ��ȸ (�ʱ�ȭ �� �� ���)
    bool OnAir;

    std::list<Bullet> bullets;

    bool leftKeyDown;
    bool rightKeyDown;

    sf::RenderWindow window;
    Level& level;

public:
    // ������ ��ġ���ް� level ������ �̿��� �浹üũ�� �ϱ����� �����Ѵ�
    Player(float x, float y, Level& level) : direction(true), width(50.0f), height(50.0f), speed(500.0f), jumpHeight(650.0f), maxJumpChance(2), jumpChance(maxJumpChance), OnAir(false), level(level), leftKeyDown(false), rightKeyDown(false)
    {
        // �Ǻ��� ����� �Ʒ�
        shape.setOrigin(width / 2, height);
        shape.setSize(sf::Vector2f(50.0f, height));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
    }

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