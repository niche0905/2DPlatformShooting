#include "Level.h"


bool Level::save(const std::string& filename)
{
	std::ofstream outFile(filename);

	// ������ ������ �ʾҴٸ�
	if (not outFile.is_open())
		return false;

	size_t platformCount = platforms.size();

	outFile << platformCount << "\n";
	for (const auto& platform : platforms) {
		outFile << platform << "\n";
	}
	
	outFile.close();

	return true;
}

bool load(const std::string& filename)
{
	return false;
}

void Level::draw(sf::RenderWindow& window)
{
	// platforms�� �ִ� ��� platform�� �׸���
	for (auto& platform : platforms) {
		platform.draw(window);
	}
}
