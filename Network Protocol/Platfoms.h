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
private:
	std::vector<StdPlatfom> platforms;

	StdLevel();

	StdLevel(const StdLevel&) = delete;
	StdLevel& operator=(const StdLevel&) = delete;
	StdLevel(StdLevel&&) = delete;
	StdLevel& operator=(StdLevel&&) = delete;
	
public:

	static StdLevel& Instance();

	bool Save(const std::string& filename);
	bool Load(const std::string& filename);

	const std::vector<StdPlatfom>& GetPlatforms() const;
	std::vector<StdPlatfom>& GetPlatforms();
};