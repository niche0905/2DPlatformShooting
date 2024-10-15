#pragma once

class Image
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

public:
	Image();

	void draw(sf::RenderWindow& window);
};

