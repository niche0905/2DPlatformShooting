#pragma once

// Dummy 객체 생성(송승호 09/03)
class Dummy {
    bool isActive;

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

    uint8_t gunId;

    std::list<Bullet> bullets;
    int curMag;
    std::chrono::system_clock::time_point lastFireTime;

    Level* level;
    Image image{ TextureID::BANANA };

public:
    Dummy() = default;

    Dummy(float x, float y, Level* level, int texture_id);

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

    // 부활하는 함수(활성화)
    void reviveDummy();

    // 총알 충돌체크(기존의 충돌체크완 다르게 충돌 유무만 처리)
    bool checkCollisionBullet(sf::FloatRect other);

    // 더미에게 데미지를 준다
    void takeDamage(bool direction, float damage);

    // 피해량을 조절해준다(업데이트)
    void damageControll(long long deltaTime);

    // 더미 포지션 set
    void setPosition(float x, float y);

    // Getter
    int getCurMag() const { return curMag; }
    int getGunID() const { return gunId; }
    int getLife() const { return life; }
};