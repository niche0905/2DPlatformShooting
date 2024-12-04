#pragma once
#include "pch.h"

class Platform
{
public:
	float x;	//����
	float y;	//��	
	float sizeX;	//���� ����
	float sizeY;	//���� ����
	sf::RectangleShape shape;

public:
	Platform();

	// ���� �� (-x, -y) ���� �ް� width, height ���� �޴´�
	Platform(float left, float top, float sizeX, float sizeY);

	// �� �÷����� �׷��ش�
	void draw(sf::RenderWindow& window);

	// �ش� �÷����� ���� �ٿ���ڽ��� �����Ѵ�
	sf::FloatRect getGlobalBounds() const;

	friend std::istream& operator>>(std::istream& is, Platform& platform);
	friend std::ostream& operator<<(std::ostream& os, const Platform& platform);
};


class Level
{
public:
	std::vector<Platform> platforms;
	float leftBound;
	float rightBound;

public:
	// ������ -> ���ڿ��� �޾Ƽ� ������ �ε��� ����
	Level() : leftBound(10000.0f), rightBound(-10000.0f)
	{
		load();
	}

	// Level�� ������ �ҷ����� �Լ�
	bool load();

	// ���� �׷��ִ� �Լ�
	void draw(sf::RenderWindow& window);
};

