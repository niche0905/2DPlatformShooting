#pragma once

// define constant
constexpr auto IMAGE_PATH{ "Resource\\Image" };


// define Enum
enum TextureID
{
	BANANA,
	END
};

constexpr const char* IMAGE_FILENAME[]
{
	"visual banana.png"
};

class Image
{
private:
	sf::Texture texture;
	sf::Sprite sprite;

public:
	Image() = delete;
	Image(int id);
	Image(int id, const float width, const float height);

	void scale(const float width, const float height);
	void draw(sf::RenderWindow& window);
};

