#pragma once
#define SFML_STATIC
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class Platform
{
public:
	float x;	//����
	float y;	//��	
	float sizeX;	//���� ����
	float sizeY;	//���� ����
	sf::RectangleShape shape;

public:
	// ���� �� (-x, -y) ���� �ް� width, height ���� �޴´�
	Platform(float left, float top, float sizeX, float sizeY) : x(left), y(top), sizeX(sizeX), sizeY(sizeY) 
	{
		// �Ǻ��� �״�� ���� ��
		shape.setSize(sf::Vector2f(sizeX, sizeY));
		shape.setPosition(x, y);
		shape.setFillColor(sf::Color::Black);
	}

	// �� �÷����� �׷��ش�
	void draw(sf::RenderWindow& window)
	{
		window.draw(shape);
	}

	// �ش� �÷����� ���� �ٿ���ڽ��� �����Ѵ�
	sf::FloatRect getGlobalBounds() const
	{
		return shape.getGlobalBounds();
	}
};


class Level
{
public:
	std::vector<Platform> platforms;

public:
	// ������ -> ���ڿ��� �޾Ƽ� ������ �ε��� ����
	Level()
	{
		platforms.push_back(Platform(0.0f, 400.0f, 800.0f, 20.0f));
		platforms.push_back(Platform(400.0f, 300.0f, 200.0f, 20.0f));
	}

	// ���� �׷��ִ� �Լ�
	void draw(sf::RenderWindow& window);
};

