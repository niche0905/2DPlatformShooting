#include "pch.h"
#include "Image.h"

Image::Image(std::string_view filename)
{
	fs::path path{ fs::path{IMAGE_PATH} / filename };
	if (!texture.loadFromFile(path.string())) {
		cout << "이미지 로드 실패\n";
		exit(-1);
	}
	sprite.setTexture(texture);
}

Image::Image(std::string_view filename, const float width, const float height)
	: Image(filename)
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
	window.draw(sprite);
}
