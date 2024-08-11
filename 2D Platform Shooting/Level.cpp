#include "Level.h"


void Level::draw(sf::RenderWindow& window)
{
	sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Black);

	for (auto& platform : platforms) {
		int sizeX = platform.x2 - platform.x1;
		int sizeY = platform.y1 - platform.y2;

		shape.setSize(sf::Vector2f(sizeX, sizeY));
		shape.setPosition(platform.x1, platform.y2);

		window.draw(shape);
	}
}
