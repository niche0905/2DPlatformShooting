#include "pch.h"
#include "Level.h"

Platform::Platform() : x(0.0f), y(0.0f), sizeX(0.0f), sizeY(0.0f), shape(sf::Vector2f(sizeX, sizeY))
{
	// 피봇은 그대로 왼쪽 위
	shape.setPosition(x, y);
	// PlatformColor를 pch에서 정의 해두었지만 정의되지 않아서 생상이 안나와서 임시로 sf::color::Black으로 설정
	shape.setFillColor(sf::Color::Black);
}

Platform::Platform(float left, float top, float sizeX, float sizeY) : x(left), y(top), sizeX(sizeX), sizeY(sizeY), shape(sf::Vector2f(sizeX, sizeY))
{
	// 피봇은 그대로 왼쪽 위
	shape.setPosition(x, y);
	// PlatformColor를 pch에서 정의 해두었지만 정의되지 않아서 생상이 안나와서 임시로 sf::color::Black으로 설정
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
	// pch에 PlatformColor를 상수로 정의해 봤지만 똑같이 문제가 생겼다
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
	// platforms에 있는 모든 platform을 그린다
	for (auto& platform : platforms) {
		platform.draw(window);
	}
}
