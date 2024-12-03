#pragma once
#include "Platform.h"


class Object;
class Platform;

class Level
{
	std::vector<Platform> platforms;
	float leftBound;
	float rightBound;

public:
	Level();

	void load();

	bool Collsion(Object& other);

};

