#include "pch.h"
#include "Level.h"

Platform::Platform() : x(0.0f), y(0.0f), sizeX(0.0f), sizeY(0.0f), shape(sf::Vector2f(sizeX, sizeY))
{
	// �Ǻ��� �״�� ���� ��
	shape.setPosition(x, y);
	// PlatformColor�� pch���� ���� �صξ����� ���ǵ��� �ʾƼ� ������ �ȳ��ͼ� �ӽ÷� sf::color::Black���� ����
	shape.setFillColor(sf::Color::Black);
}

Platform::Platform(float left, float top, float sizeX, float sizeY) : x(left), y(top), sizeX(sizeX), sizeY(sizeY), shape(sf::Vector2f(sizeX, sizeY))
{
	// �Ǻ��� �״�� ���� ��
	shape.setPosition(x, y);
	// PlatformColor�� pch���� ���� �صξ����� ���ǵ��� �ʾƼ� ������ �ȳ��ͼ� �ӽ÷� sf::color::Black���� ����
	shape.setFillColor(sf::Color::Black);
}

void Platform::draw(sf::RenderWindow& window)
{
	window.draw(shape);
}

sf::FloatRect Platform::getGlobalBounds() const
{
	return shape.getGlobalBounds();
}

std::istream& operator>>(std::istream& is, Platform& platform)
{
	is >> platform.x >> platform.y >> platform.sizeX >> platform.sizeY;
	platform.shape.setSize(sf::Vector2f(platform.sizeX, platform.sizeY));
	platform.shape.setPosition(platform.x, platform.y);
	// pch�� PlatformColor�� ����� ������ ������ �Ȱ��� ������ �����
	platform.shape.setFillColor(sf::Color::Black);
	return is;
}

std::ostream& operator<<(std::ostream& os, const Platform& platform)
{
	os << platform.x << " " << platform.y << " " << platform.sizeX << " " << platform.sizeY;

	return os;
}

/*
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
*/

bool Level::load()
{
	std::vector<StdPlatfom> vec = StdLevel::Instance().GetPlatforms();

	platforms.clear();
	platforms.reserve(vec.size());
	for (StdPlatfom& SP : vec) {
		leftBound = leftBound > SP.posX ? SP.posX : leftBound;
		rightBound = rightBound < SP.posX + SP.width ? SP.posX + SP.width : rightBound;

		platforms.emplace_back(SP.posX, SP.posY, SP.width, SP.height);
	}

	return true;
}

void Level::draw(sf::RenderWindow& window)
{
	// platforms�� �ִ� ��� platform�� �׸���
	for (auto& platform : platforms) {
		platform.draw(window);
	}
}
