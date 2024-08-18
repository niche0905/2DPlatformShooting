#include "Level.h"


bool Level::save(const std::string& filename)
{
	std::ofstream outFile(filename);

	// 파일이 열리지 않았다면
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
	// platforms에 있는 모든 platform을 그린다
	for (auto& platform : platforms) {
		platform.draw(window);
	}
}
