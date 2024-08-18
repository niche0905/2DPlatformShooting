#pragma once
#define SFML_STATIC
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>


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
	Platform(float left, float top, float sizeX, float sizeY) : x(left), y(top), sizeX(sizeX), sizeY(sizeY), shape(sf::Vector2f(sizeX, sizeY))
	{
		// �Ǻ��� �״�� ���� ��
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

	friend std::istream& operator>>(std::istream& is, Platform& platform);
	friend std::ostream& operator<<(std::ostream& os, const Platform& platform);
};

std::istream& operator>>(std::istream& is, Platform& platform)
{
	is >> platform.x >> platform.y >> platform.sizeX >> platform.sizeY;
	platform.shape.setSize(sf::Vector2f(platform.sizeX, platform.sizeY));
	platform.shape.setPosition(platform.x, platform.y);

	return is;
}

std::ostream& operator<<(std::ostream& os, const Platform& platform)
{
	os << platform.x << " " << platform.y << " " << platform.sizeX << " " << platform.sizeY;

	return os;
}


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

	// Level�� ������ �����ϴ� �Լ�
	bool save(const std::string& filename);

	// Level�� ������ �ҷ����� �Լ�
	bool load(const std::string& filename);

	// ���� �׷��ִ� �Լ�
	void draw(sf::RenderWindow& window);
};

