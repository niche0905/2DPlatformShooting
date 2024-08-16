#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Level.h"

constexpr float GravityAcc = 0.9810f;
constexpr float PlatformUp = 10.0f;


class Player {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;

    float width;
    float height;
    float speed;

    float jumpHeight;
    bool OnAir;

    sf::RenderWindow window;
    Level& level;

public:
    // 생성할 위치를받고 level 정보를 이용해 충돌체크를 하기위해 저장한다
    Player(float x, float y, Level& level) : width(50.0f), height(50.0f), speed(500.0f), jumpHeight(750.0f), OnAir(false), level(level)
    {
        // 피봇은 가운대 아래
        shape.setOrigin(width / 2, height);
        shape.setSize(sf::Vector2f(50.0f, height));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
    }

    // 플레이어의 Input을 처리한다
    void handleInput();

    // 플레이어의 위치를 업데이트한다
    void update(long long deltaTime); 

    // 플레이어를 그린다
    void draw(sf::RenderWindow& window);

    // 월드에서의 바운딩박스를 얻는다
    sf::FloatRect getGlobalBounds() const;

    // 충돌체크를 한다
    bool checkCollision(sf::FloatRect other);

    // 플레이어 위치(피봇 점)을 찾는 함수
    sf::Vector2f getPosition() const;
};