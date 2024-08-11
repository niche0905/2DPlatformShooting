#pragma once
#define SFML_STATIC
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Platform
{
public:
	float x1;	//왼쪽			┌----------┐ (x2, y2)
	float y1;	//아래			|		   |
	float x2;	//오른쪽			|		   |
	float y2;	//위		(x1, y1)└----------┘

public:
	Platform(float x1, float y1, float x2, float y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

class Level
{
public:
	std::vector<Platform> platforms;

public:
	Level()
	{
		platforms.push_back(Platform(0, 600.0f, 800.0f, 400.0f));
	}

	void draw(sf::RenderWindow& window);
};

