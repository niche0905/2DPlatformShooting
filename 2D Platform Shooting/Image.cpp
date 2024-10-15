#include "pch.h"
#include "Image.h"

Image::Image()
{
	if (!texture.loadFromFile("Resource\\Image\\visual banana.png")) {
		cout << "�̹��� �ε� ����\n";
		exit(-1);
	}
	sprite.setTexture(texture);
}

void Image::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}
