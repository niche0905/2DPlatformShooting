#pragma once

// define constant
constexpr auto IMAGE_PATH{ "Resource\\Image" };


// define Enum
enum TextureID
{
	BANANA,
	PLAYER1,
	PLAYER2,
	BACKGROUND,
	UI1,
	UI2,
	TITLE,
	P1WIN,
	P2WIN,
	END
};

constexpr const char* IMAGE_FILENAME[]
{
	"visual banana.png",
	"Char1\\idle_0.png",
	"Char2\\idle_0.png",
	"Background.jpg",
	"ui1.png",
	"ui2.png",
	"Title.png",
	"p1win.png",
	"p2win.png"
};

class Image
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
	bool reversed{ false };
	bool show{ true };

public:
	Image() = delete;
	Image(int id);
	Image(int id, const float width, const float height);

	void scale(const float width, const float height);
	void SetPosition(const float x, const float y);
	void SetPosition(const sf::Vector2f& vec);
	void SetReversed(const bool value);
	void SetShow(const bool value) { show = value; }
	void draw(sf::RenderWindow& window);
	void drawFixed(sf::RenderWindow& window);


};

