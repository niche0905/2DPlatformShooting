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

bool Level::load(const std::string& filename)
{
	std::ifstream inFile(filename);

	// ������ ������ �ʾҴٸ� load ���� ��ȯ
	if (!inFile.is_open())
		return false;

	platforms.clear();

	size_t platformCount;

	inFile >> platformCount;
	platforms.reserve(platformCount);

	for (size_t i = 0; i < platformCount; ++i) {
		Platform platform;
		inFile >> platform;
		platforms.push_back(platform);
	}

	inFile.close();

	return true;
}

void Level::draw(sf::RenderWindow& window)
{
	// platforms�� �ִ� ��� platform�� �׸���
	for (auto& platform : platforms) {
		platform.draw(window);
	}
}
