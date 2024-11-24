#include "pch.h"
#include "Level.h"


Level::Level()
	: leftBound{ 10000.0f }
	, rightBound{ -10000.0f }
{

}

void Level::load()
{
	std::vector<StdPlatfom> vec = StdLevel::Instance().GetPlatforms();

	platforms.clear();
	platforms.reserve(vec.size());
	for (StdPlatfom& SP : vec) {
		leftBound = leftBound > SP.posX ? SP.posX : leftBound;
		rightBound = rightBound < SP.posX + SP.width ? SP.posX + SP.width : rightBound;

		platforms.emplace_back(SP.posX, SP.posY, SP.width, SP.height);
	}
}

bool Level::Collsion(const Object& other)
{
	return false;
}
