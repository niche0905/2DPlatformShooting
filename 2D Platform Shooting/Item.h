#pragma once
#include "Level.h"

class Item {
private:
    bool isActive;

    uint32_t itemID;

    sf::RectangleShape shape;
    sf::Vector2f velocity;

    float width;
    float height;
    bool OnAir;

    Level* level;

public:
    // 생성자
    Item(uint32_t i_id, float x, float y, Level* level);

    // update
    void update(long long deltaTime);

    // draw
    void draw(sf::RenderWindow& window);

    // 플랫폼과 충돌 처리
    bool checkCollision(sf::FloatRect other);

    // 아이템과 충돌처리: 확인 해봐야 할듯
    bool checkCollisionBullet(sf::FloatRect other);

    // Getter
    sf::Vector2f getPosition() const;
    uint32_t GetItemId() const { return itemID; }
};