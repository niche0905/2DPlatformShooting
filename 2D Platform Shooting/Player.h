#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Level.h"


class Player {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float width;
    float height;
    float speed;
    bool isJumping;
    float jumpHeight;
    sf::RenderWindow window;
    Level& level;

public:
    // ������ ��ġ���ް� level ������ �̿��� �浹üũ�� �ϱ����� �����Ѵ�
    Player(float x, float y, Level& level) : width(50.0f), height(50.0f), speed(500.0f), isJumping(false), jumpHeight(750.0f), level(level)
    {
        // �Ǻ��� ����� �Ʒ�
        shape.setOrigin(width / 2, height);
        shape.setSize(sf::Vector2f(50.0f, height));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
    }

    // �÷��̾��� ��ġ�� ������Ʈ�Ѵ�
    void update(long long deltaTime); 

    // �÷��̾ �׸���
    void draw(sf::RenderWindow& window);

    // ���忡���� �ٿ���ڽ��� ��´�
    sf::FloatRect getGlobalBounds() const;

    // �浹üũ�� �Ѵ�
    bool CheckCollision(sf::FloatRect other);
};