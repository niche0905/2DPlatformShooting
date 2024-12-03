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

		// Test
		platforms.emplace_back(SP.posX, SP.posY, SP.width, 1000.0f);
	}
}

bool Level::Collsion(Object& other)
{
	for (Platform& platform : platforms) {
		if (platform.Collision(other)) {
			// Logging
			cout << "Block\n";
			other.SetPos(other.GetPos().posX, platform.GetBB().top);
			return true;
		}
	}

	return false;
}
