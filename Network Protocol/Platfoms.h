#pragma once

const char LevelPath[] = "../Saved/Levels/";


struct StdPlatfom
{
	float posX, posY;
	float width, height;

	friend std::istream& operator>>(std::istream& is, StdPlatfom& platform);
	friend std::ostream& operator<<(std::ostream& os, const StdPlatfom& platform);
};

class StdLevel
{
public:
	std::vector<StdPlatfom> platforms;
	
public:
	StdLevel();

	bool Save(const std::string& filename);
	bool Load(const std::string& filename);
};