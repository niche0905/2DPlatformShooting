#pragma once
#define SFML_STATIC
#include <vector>
#include <fstream>
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
	Platform() : x(0.0f), y(0.0f), sizeX(0.0f), sizeY(0.0f), shape(sf::Vector2f(sizeX, sizeY))
	{
		// �Ǻ��� �״�� ���� ��
		shape.setPosition(x, y);
		shape.setFillColor(sf::Color::Black);
	}

	// ���� �� (-x, -y) ���� �ް� width, height ���� �޴´�
	Platform(float left, float top, float sizeX, float sizeY) : x(left), y(top), sizeX(sizeX), sizeY(sizeY), shape(sf::Vector2f(sizeX, sizeY))
	{
		// �Ǻ��� �״�� ���� ��
		shape.setPosition(x, y);
		shape.setFillColor(sf::Color::Black);
	}

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

public:
	// ������ -> ���ڿ��� �޾Ƽ� ������ �ε��� ����
	Level()
	{
		platforms.push_back(Platform(-200.0f, 400.0f, 1000.0f, 20.0f));
		platforms.push_back(Platform(100.0f, 250.0f, 400.0f, 20.0f));
		platforms.push_back(Platform(-100.0f, 100.0f, 300.0f, 20.0f));
		platforms.push_back(Platform(400.0f, 100.0f, 300.0f, 20.0f));

		save("Level1");
	}

	// Level�� ������ �����ϴ� �Լ�
	bool save(const std::string& filename);

	// Level�� ������ �ҷ����� �Լ�
	bool load(const std::string& filename);

	// ���� �׷��ִ� �Լ�
	void draw(sf::RenderWindow& window);
};

