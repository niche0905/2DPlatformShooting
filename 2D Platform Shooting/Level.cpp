#include "Level.h"


void Level::draw(sf::RenderWindow& window)
{
	// platforms에 있는 모든 platform을 그린다
	for (auto& platform : platforms) {
		platform.draw(window);
	}
}
