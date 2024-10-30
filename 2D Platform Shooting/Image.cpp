#include "pch.h"
#include "Image.h"

Image::Image(int id)
{
	fs::path path{ fs::path{IMAGE_PATH} / IMAGE_FILENAME[id]};
	if (!texture.loadFromFile(path.string())) {
		cout << "이미지 로드 실패\n";
		exit(-1);
	}
}

Image::Image(int id, const float width, const float height)
	: Image(id)
{
	scale(width, height);
}

void Image::scale(const float width, const float height)
{
	sf::Vector2u textureSize = texture.getSize();
	sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
	sprite.setScale( width / textureSize.x, height / textureSize.y );
}

void Image::draw(sf::RenderWindow& window)
{
	sprite.setTexture(texture);
	window.draw(sprite);
}
