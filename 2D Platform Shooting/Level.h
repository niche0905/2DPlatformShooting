#pragma once
#define SFML_STATIC
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class Platform
{
public:
	float x;	//왼쪽
	float y;	//위	
	float sizeX;	//가로 길이
	float sizeY;	//세로 길이
	sf::RectangleShape shape;

public:
	// 왼쪽 위 (-x, -y) 값을 받고 width, height 값을 받는다
	Platform(float left, float top, float sizeX, float sizeY) : x(left), y(top), sizeX(sizeX), sizeY(sizeY) 
	{
		// 피봇은 그대로 왼쪽 위
		shape.setSize(sf::Vector2f(sizeX, sizeY));
		shape.setPosition(x, y);
		shape.setFillColor(sf::Color::Black);
	}

	// 이 플랫폼을 그려준다
	void draw(sf::RenderWindow& window)
	{
		window.draw(shape);
	}

	// 해당 플랫폼의 월드 바운딩박스를 리턴한다
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
	// 생성자 -> 문자열을 받아서 파일을 로드할 예정
	Level()
	{
		platforms.push_back(Platform(0.0f, 400.0f, 800.0f, 200.0f));
		platforms.push_back(Platform(400.0f, 300.0f, 200.0f, 100.0f));
	}

	// 맵을 그려주는 함수
	void draw(sf::RenderWindow& window);
};

