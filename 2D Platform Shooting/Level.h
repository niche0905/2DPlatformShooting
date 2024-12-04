#pragma once
#include "pch.h"

class Platform
{
public:
	float x;	//왼쪽
	float y;	//위	
	float sizeX;	//가로 길이
	float sizeY;	//세로 길이
	sf::RectangleShape shape;

public:
	Platform();

	// 왼쪽 위 (-x, -y) 값을 받고 width, height 값을 받는다
	Platform(float left, float top, float sizeX, float sizeY);

	// 이 플랫폼을 그려준다
	void draw(sf::RenderWindow& window);

	// 해당 플랫폼의 월드 바운딩박스를 리턴한다
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
	// 생성자 -> 문자열을 받아서 파일을 로드할 예정
	Level() : leftBound(10000.0f), rightBound(-10000.0f)
	{
		load();
	}

	// Level의 정보를 불러오는 함수
	bool load();

	// 맵을 그려주는 함수
	void draw(sf::RenderWindow& window);
};

