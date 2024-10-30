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
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2.f, bounds.height);
	sf::Vector2u textureSize = texture.getSize();
	sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
	sprite.setScale( width / textureSize.x, height / textureSize.y );
}

void Image::SetPosition(const float x, const float y)
{
	sprite.setPosition(x, y);
}

void Image::SetPosition(const sf::Vector2f& vec)
{
	sprite.setPosition(vec);
}

void Image::SetReversed(const bool value)
{
	reversed = value;
}

void Image::draw(sf::RenderWindow& window)
{
	sprite.setTexture(texture);
	if (reversed) {
		sf::FloatRect bounds = sprite.getLocalBounds();
		std::cout << bounds.width << bounds.width << endl;
		sprite.setOrigin(bounds.width / 2.f, bounds.height);
		sprite.setScale(-0.1f, 0.1f);
		window.draw(sprite);
		sprite.setScale(-0.1f, 0.1f);
	}
	else {
		window.draw(sprite);
	}
}
