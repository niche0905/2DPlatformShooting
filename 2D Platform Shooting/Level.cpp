#include "Level.h"


void Level::draw(sf::RenderWindow& window)
{
	// platforms�� �ִ� ��� platform�� �׸���
	for (auto& platform : platforms) {
		platform.draw(window);
	}
}
