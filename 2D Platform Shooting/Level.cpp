#include "Level.h"


void Level::draw(sf::RenderWindow& window)
{
	sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Black);

	for (auto& platform : platforms) {
		platform.draw(window);
	}
}
