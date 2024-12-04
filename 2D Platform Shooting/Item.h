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
    // ������
    Item(uint32_t i_id, float x, float y, Level* level);

    // update
    void update(long long deltaTime);

    // draw
    void draw(sf::RenderWindow& window);

    // �÷����� �浹 ó��
    bool checkCollision(sf::FloatRect other);

    // �����۰� �浹ó��: Ȯ�� �غ��� �ҵ�
    bool checkCollisionBullet(sf::FloatRect other);

    // Getter
    sf::Vector2f getPosition() const;
    uint32_t GetItemId() const { return itemID; }
};