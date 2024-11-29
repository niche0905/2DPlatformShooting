#include "pch.h"
#include "Item.h"
#include "Level.h"
#include "Utilities.h"

Item::Item(uint32_t i_id, float x, float y, Level* level) : isActive(true), itemID{i_id}, width(35.0f), height(50.0f), OnAir(true), level(level)
{
    // 피봇은 가운대 아래
    shape.setOrigin(width / 2, height);
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color::Red);
}

void Item::update(long long deltaTime)
{
    if (not isActive) return;

    if (OnAir) {
        velocity.y += GravityAcc * GravityMul * (deltaTime / myNP::microToSecond);
    }

    // 속도만큼 움직임
    shape.move((velocity) * (deltaTime / myNP::microToSecond));

    bool noOnePlatformCollide = true;

    // 하나라도 밟고있는 플랫폼이 있는지 체크
    for (const auto& platform : level->platforms) {
        if (checkCollision(platform.getGlobalBounds())) {
            OnAir = false;
            velocity.y = 0.0f;
            shape.setPosition(shape.getPosition().x, platform.y);
            noOnePlatformCollide = false;
        }
    }



    // 플랫폼에 붙어있지 않다면 공중 판정
    if (noOnePlatformCollide) {
        OnAir = true;
    }
}

void Item::draw(sf::RenderWindow& window)
{
    if (not isActive) return;   // 활성화 상태가 아니라면 Draw 종료

    window.draw(shape);
}

bool Item::checkCollision(sf::FloatRect other)
{
    sf::FloatRect itemBounds = shape.getGlobalBounds();
    if (itemBounds.intersects(other) and velocity.y > 0) {
        float itemBottom = itemBounds.top + itemBounds.height;
        float platformTop = other.top;

        if (itemBottom <= platformTop + PlatformUp) {
            return true;
        }
    }

    return false;
}

sf::Vector2f Item::getPosition() const
{
    return shape.getPosition();
}

bool Item::checkCollisionBullet(sf::FloatRect other)
{
    sf::FloatRect itemBounds = shape.getGlobalBounds();
    if (itemBounds.intersects(other)) {
        return true;
    }

    return false;
}