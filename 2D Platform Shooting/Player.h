#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <chrono>
#include "Level.h"
#include "Bullet.h"
#include "Gun.h"

constexpr float GravityAcc = 9.810f;            // 중력 가속도
constexpr float GravityMul = 120.0f;            // 중력 가속도에 곱하는 수(게임성을 위해)
constexpr float PlatformUp = 10.0f;             // 플랫폼 위에 있다는 기준(충돌 처리 시 사용)

constexpr float DamageScalingRatio = 10.0f;     // 피해량 정상화 상수(공기저항 역할)


class Player {
private:
    bool isActive;      // 플레이어 활성화 비활성화를 나타내는 변수

    sf::RectangleShape shape;
    sf::Vector2f velocity;

    bool direction;     // true : Left(←),	false : Right(→)
    float width;
    float height;
    float speed;

    float jumpHeight;
    int jumpChance;     // 점프 기회
    int maxJumpChance;  // 최대 점프 기회 (초기화 할 때 사용)
    bool OnAir;


    // [cham] 9.22
    uint8_t gunId;
    
    std::list<Bullet> bullets;
    std::chrono::system_clock::time_point lastFireTime;     // 총을 발사한 마지막 시간    

    bool leftKeyDown;
    bool rightKeyDown;

    sf::RenderWindow window;
    Level& level;

public:
    // 생성할 위치를받고 level 정보를 이용해 충돌체크를 하기위해 저장한다
    Player(float x, float y, Level& level) : isActive(true), direction(true), width(50.0f), height(50.0f), speed(500.0f), jumpHeight(650.0f), maxJumpChance(2), jumpChance(maxJumpChance), OnAir(false), level(level), leftKeyDown(false), rightKeyDown(false)
    {
        // 피봇은 가운대 아래
        shape.setOrigin(width / 2, height);
        shape.setSize(sf::Vector2f(50.0f, height));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);

        // 총의 발사속도 제한을 위한 변수 초기화
        lastFireTime = std::chrono::system_clock::now();
    }

    // 플레이어의 Input을 처리한다
    void handleInput(const sf::Event& event);

    // 플레이어가 총을 발사한다.
    void fireBullet();

    // 플레이어의 위치를 업데이트한다
    void update(long long deltaTime); 

    // 플레이어가 발사한 총알들을 업데이트 해준다
    void updateBullets(long long deltaTime);

    // 플레이어를 그린다
    void draw(sf::RenderWindow& window);

    // 월드에서의 바운딩박스를 얻는다
    sf::FloatRect getGlobalBounds() const;

    // 충돌체크를 한다
    bool checkCollision(sf::FloatRect other);

    // 플레이어 위치(피봇 점)을 찾는 함수
    sf::Vector2f getPosition() const;

    // 플레이어 대쉬(민경원 테스트)
    void dash();

    // 총알들이 적을 맞췄는지 검사
    void hitTheEnemy(class Dummy& dummy);

    // 부활하는 함수(활성화)
    void revivePlayer();
    
};


// Dummy 객체 생성(송승호 09/03)
class Dummy {
    sf::RectangleShape shape;
    sf::Vector2f velocity;

    float damaged;      // 피해 입은 양 -(음수) 왼쪽으로 힘을 받음 +(양수) 오른쪽으로 힘을 받음

    bool direction;     // true : Left(←),	false : Right(→)
    float width;
    float height;
    float speed;

    float jumpHeight;
    int jumpChance;     // 점프 기회
    int maxJumpChance;  // 최대 점프 기회 (초기화 할 때 사용)
    bool OnAir;

    bool leftKeyDown;
    bool rightKeyDown;

    sf::RenderWindow window;
    Level& level;

public:
    // 생성할 위치를받고 level 정보를 이용해 충돌체크를 하기위해 저장한다
    Dummy(float x, float y, Level& level) : direction(true), width(50.0f), height(50.0f), speed(500.0f), jumpHeight(650.0f), maxJumpChance(2), jumpChance(maxJumpChance), OnAir(false), level(level), leftKeyDown(false), rightKeyDown(false)
    {
        // 피봇은 가운대 아래
        shape.setOrigin(width / 2, height);
        shape.setSize(sf::Vector2f(50.0f, height));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Red);
    }

    // 더미의 Input을 처리한다
    void handleInput(const sf::Event& event);

    // 피해량을 조절해준다(업데이트)
    void damageControll(long long deltaTime);

    // 더미의 위치를 업데이트한다
    void update(long long deltaTime);

    // 더미를 그린다
    void draw(sf::RenderWindow& window);

    // 월드에서의 바운딩박스를 얻는다
    sf::FloatRect getGlobalBounds() const;

    // 충돌체크를 한다
    bool checkCollision(sf::FloatRect other);

    // 더미의 위치(피봇 점)을 찾는 함수
    sf::Vector2f getPosition() const;

    // 총알 충돌체크(기존의 충돌체크완 다르게 충돌 유무만 처리)
    bool checkCollisionBullet(sf::FloatRect other);

    // 더미에게 데미지를 준다
    void takeDamage(bool direction, float damage);
};