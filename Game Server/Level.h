#pragma once

class Object;
class Platform;

class Level
{
	std::vector<Platform> platforms;
	float leftBound;
	float rightBound;

public:
	void load();

	bool Collsion(const Object& other);

};

