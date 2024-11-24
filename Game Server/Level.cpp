#include "pch.h"
#include "Level.h"


Level::Level()
	: leftBound{0.0f}
	, rightBound{0.0f}
{

}

void Level::load()
{
	std::vector<StdPlatfom> vec = StdLevel::Instance().GetPlatforms();

	platforms.clear();
	platforms.reserve(vec.size());
	for (StdPlatfom& SP : vec) {
		platforms.emplace_back(SP.posX, SP.posY, SP.width, SP.height);
	}
}

bool Level::Collsion(const Object& other)
{
	return false;
}
