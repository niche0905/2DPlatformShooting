#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Level.h"

constexpr float GravityAcc = 9.810f;
constexpr float GravityMul = 120.0f;
constexpr float PlatformUp = 10.0f;


class Player {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;

    float width;
    float height;
    float speed;

    float jumpHeight;
    int jumpChance; // ���� ��ȸ
    int maxJumpChance; // �ִ� ���� ��ȸ (�ʱ�ȭ �� �� ���)
    bool OnAir;

    bool leftKeyDown;
    bool rightKeyDown;

    sf::RenderWindow window;
    Level& level;

public:
    // ������ ��ġ���ް� level ������ �̿��� �浹üũ�� �ϱ����� �����Ѵ�
    Player(float x, float y, Level& level) : width(50.0f), height(50.0f), speed(500.0f), jumpHeight(650.0f), maxJumpChance(2), jumpChance(maxJumpChance), OnAir(false), level(level), leftKeyDown(false), rightKeyDown(false)
    {
        // �Ǻ��� ����� �Ʒ�
        shape.setOrigin(width / 2, height);
        shape.setSize(sf::Vector2f(50.0f, height));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
    }

    // �÷��̾��� Input�� ó���Ѵ�
    void handleInput(const sf::Event& event);

    // �÷��̾��� ��ġ�� ������Ʈ�Ѵ�
    void update(long long deltaTime); 

    // �÷��̾ �׸���
    void draw(sf::RenderWindow& window);

    // ���忡���� �ٿ���ڽ��� ��´�
    sf::FloatRect getGlobalBounds() const;

    // �浹üũ�� �Ѵ�
    bool checkCollision(sf::FloatRect other);

    // �÷��̾� ��ġ(�Ǻ� ��)�� ã�� �Լ�
    sf::Vector2f getPosition() const;
};