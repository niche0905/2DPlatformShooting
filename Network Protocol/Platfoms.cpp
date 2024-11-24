#include "pch.h"
#include "Platfoms.h"
#include <string>
#include <fstream>


std::istream& operator>>(std::istream& is, StdPlatfom& platform)
{
	is >> platform.posX >> platform.posY >> platform.width >> platform.height;

	return is;
}

std::ostream& operator<<(std::ostream& os, const StdPlatfom& platform)
{
	os << platform.posX << " " << platform.posY << " " << platform.width << " " << platform.height;

	return os;
}

StdLevel::StdLevel()
{
	Load("Level1");
}

StdLevel& StdLevel::Instance()
{
	static StdLevel instance;
	return instance;
}

bool StdLevel::Save(const std::string& filename)
{
	std::string filePath = LevelPath + filename;

	std::ofstream outFile(filePath);

	if (not outFile.is_open())
		return false;	// 파일을 열지 못해서 실패

	size_t platform_number = platforms.size();

	outFile << platform_number << "\n";
	for (const auto& platform : platforms) {
		outFile << platform << "\n";
	}

	outFile.close();

	return true;
}

bool StdLevel::Load(const std::string& filename)
{
	std::string filePath = LevelPath + filename;

	std::ifstream inFile(filePath);

	if (not inFile.is_open())
		return false;	// 파일을 열지 못해서 실패

	platforms.clear();

	size_t platform_number;

	inFile >> platform_number;
	platforms.reserve(platform_number);

	for (size_t i = 0; i < platform_number; ++i) {
		StdPlatfom platform;
		inFile >> platform;
		platforms.push_back(platform);
	}

	inFile.close();
	
	return true;
}

const std::vector<StdPlatfom>& StdLevel::GetPlatforms() const
{
	return platforms;
}

std::vector<StdPlatfom>& StdLevel::GetPlatforms()
{
	return platforms;
}
