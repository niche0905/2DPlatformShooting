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
	bool direction;	// true : Left(��),	false : Right(��)
	float speed;
	float damage;

	sf::RectangleShape shape;

public:
	// ������ �� ����� �߻��� ��ġ�� �޾ƾ� �Ѵ�
	Bullet(bool direct, sf::Vector2f pos) : direction(direct), speed(BulletSpeed), damage(BulletDamage)
	{
		shape.setSize(sf::Vector2f(BulletWidth, BulletHeight));
		shape.setOrigin(BulletWidth / 2, BulletHeight / 2);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color::Yellow);
	}

	// �Ѿ��� ��ġ�� ���� ������ �Ѿ������� �Ǵ��Ѵ�
	bool isOutBounds(float left, float right);

	// �Ѿ��� ��ġ�� ������Ʈ �Ѵ�
	void update(long long deltaTime);

	// �Ѿ��� �׷��ش�
	void draw(sf::RenderWindow& window) const;

};

