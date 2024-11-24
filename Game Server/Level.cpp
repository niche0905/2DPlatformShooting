#include "pch.h"
#include "Level.h"


Level::Level()
{

}

void Level::load()
{
	std::vector<StdPlatfom> vec = StdLevel::Instance().GetPlatforms();

	for (StdPlatfom& SP : vec) {
		std::cout << SP.posX << " " << SP.posY << "\n";
	}
}

bool Level::Collsion(const Object& other)
{
	return false;
}
