#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

constexpr float BulletWidth = 20.0f;
constexpr float BulletHeight = 6.0f;

constexpr float BulletSpeed = 600.0f;
constexpr float BulletDamage = 10.0f;


class Bullet
{
private:
	bool direction;	// true : Left(←),	false : Right(→)
	float speed;
	float damage;

	sf::RectangleShape shape;

public:
	// 생성할 때 방향과 발사한 위치를 받아야 한다
	Bullet(bool direct, sf::Vector2f pos) : direction(direct), speed(BulletSpeed), damage(BulletDamage)
	{
		shape.setSize(sf::Vector2f(BulletWidth, BulletHeight));
		shape.setOrigin(BulletWidth / 2, BulletHeight / 2);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color::Yellow);
	}

	// 총알의 위치가 영역 밖으로 넘었는지를 판단한다
	bool isOutBounds(float left, float right);

	// 총알의 위치를 업데이트 한다
	void update(long long deltaTime);

	// 총알을 그려준다
	void draw(sf::RenderWindow& window) const;

};

