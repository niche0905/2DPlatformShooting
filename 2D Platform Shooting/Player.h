#pragma once
#include "Bullet.h"
#include "Level.h"
#include "Image.h"

constexpr float DamageScalingRatio = 50.0f;     // 피해량 정상화 상수(공기저항 역할)
constexpr float DamagePower = 5.0f;             // 데미지 받을 때 수치 조정값
constexpr float FrictionScale = 5.0f;           // 데미지를 감소 시킬 때 마찰력
constexpr float AirFrictionScale = 3.0f;        // 데미지를 감소 시킬 때 공기 정항력
// 송승호 코멘트 09/26
// 아직 값이 만족스럽진 않음
// 선형적으로 데미지를 조정하는게 아닌 다른 방식도 탐구해볼 필요 느낌


class Player {
private:
    bool isActive;      // 플레이어 활성화 비활성화를 나타내는 변수

    sf::RectangleShape shape;
    sf::Vector2f velocity;

    bool direction;     // true : Left(←),	false : Right(→)
    float width;
    float height;
    float speed;

    float damaged;      // 피해 입은 양 -(음수) 왼쪽으로 힘을 받음 +(양수) 오른쪽으로 힘을 받음

    float jumpHeight;
    int jumpChance;     // 점프 기회
    int maxJumpChance;  // 최대 점프 기회 (초기화 할 때 사용)
    bool OnAir;
    int life;

    // 키입력 분리
    sf::Keyboard::Key upKeyBind;
    sf::Keyboard::Key downKeyBind;
    sf::Keyboard::Key leftKeyBind;
    sf::Keyboard::Key rightKeyBind;
    sf::Keyboard::Key attackKeyBind;

    // [cham] 9.22
    uint8_t gunId;
    
    std::list<Bullet> bullets;
    int curMag;
    std::chrono::system_clock::time_point lastFireTime;     // 총을 발사한 마지막 시간

    bool fireKeyDown;
    bool leftKeyDown;
    bool rightKeyDown;

    Level* level;
    Image image{ TextureID::BANANA };

public:
    Player() = default;

    // 생성할 위치를받고 level 정보를 이용해 충돌체크를 하기위해 저장한다
    Player(float x, float y, Level* level, int texture_id);

    // 생성할 위치와 level 정보를 받고 Key 바인딩을 위한 값들을 입력받는다
    Player(float x, float y, Level* level, sf::Keyboard::Key upKey, sf::Keyboard::Key downKey, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key attackKey, int texture_id);

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

    // 부활하는 함수(활성화)
    void revivePlayer();

    // 충돌처리 (기존의 플랫폼 충돌 처리와 다름)
    bool checkCollisionBullet(sf::FloatRect other);

    // 플레이어에게 데미지를 입힘
    void takeDamage(bool direction, float damage);

    // 피해량을 조절해준다(업데이트에서 호출)
    void damageControll(long long deltaTime);

    void setPosition(float x, float y);

    void getItem();

    // Getter
    int getCurMag() const { return curMag; }
    int getGunID() const { return gunId; }
    int getLife() const { return life; }

};


// Dummy 객체 생성(송승호 09/03)
class Dummy {
    bool isActive;

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

    // 더미의 Input을 처리한다
    void handleInput(const sf::Event& event);

    // 더미가 총을 발사한다.
    void fireBullet();

    // 더미의 위치를 업데이트한다
    void update(long long deltaTime);

    // 더미가 발사한 총알 업데이트
    void updateBullets(long long deltaTime);

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

    // 피해량을 조절해준다(업데이트)
    void damageControll(long long deltaTime);

    void setPosition(float x, float y);

    void getItem();

    // Getter
    int getCurMag() const { return curMag; }
    int getGunID() const { return gunId; }
    int getLife() const { return life; }

};