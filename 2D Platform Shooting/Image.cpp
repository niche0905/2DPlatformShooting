#include "pch.h"
#include "Image.h"

Image::Image()
{
	if (!texture.loadFromFile("Resource\\Image\\visual banana.png")) {
		cout << "이미지 로드 실패\n";
		exit(-1);
	}
	sprite.setTexture(texture);
}

void Image::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}
