#pragma once

class Image
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

public:
	Image() = delete;
	Image(std::string_view filename);
	Image(std::string_view fliename, const float width, const float height);

	void scale(const float width, const float height);
	void draw(sf::RenderWindow& window);
};

