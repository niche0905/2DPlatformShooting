#include "pch.h"
#include "GunLoader.h"
#include <random>
#include <ranges>
#include <fstream>


std::ostream& operator<<(std::ostream& os, const Gun& gun)
{
	os << "[ " << gun.name << " ]" << std::endl;
	os << "Damage: " << gun.damage << std::endl;
	os << "Speed: " << gun.speed << std::endl;
	os << "Mag: " << gun.mag << std::endl;
	os << "RPM: " << gun.RPM << std::endl;
	os << "Ratio: " << gun.ratio << std::endl;

	return os;
}

std::istream& operator>>(std::istream& is, Gun& gun)
{
	std::string temp;
	is >> temp >> gun.name >> temp;
	is >> temp >> gun.damage;
	is >> temp >> gun.speed;
	is >> temp >> gun.mag;
	is >> temp >> gun.RPM;
	is >> temp >> gun.ratio;

	return is;
}

GunLoader::GunLoader()
{
	while (not loadGunFromFile(GunSavePath))
	{
		// retry...
		// Gun 정보를 읽어야 사용할 수 있음 (게임이 진행 가능)
	} 
}

const int GunLoader::getRandomGunId()
{
	int total{};

	// ratio를 모두 더한다.
	std::ranges::for_each(gun_table | std::views::values, [&total](const auto& gun) {
		total += gun.ratio > 0 ? gun.ratio : 0;
		});

	// 난수를 생성한다.
	std::uniform_int_distribution dist(1, total);
	int random_value{ dist(RANDOM_ENGINE) };

	// ratio에 따라 값을 반환한다.
	int cnt{};
	for (const auto& gun : gun_table | std::views::values) {
		random_value -= gun.ratio > 0 ? gun.ratio : 0;
		if (random_value <= 0) { break; }
		++cnt;
	}

	return cnt;
}

bool GunLoader::loadGunFromFile(const std::string& filePath)
{
	std::ifstream inFile(filePath);
	if (not inFile.is_open()) {
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return false;
	}

	Gun gun;

	uint8_t cnt{};
	while (inFile >> gun)
	{
		gun.id = cnt;
		gun_table[cnt++] = gun;
	}

	inFile.close();

	return true;
}

void GunLoader::saveGunFromFile(const std::string& filePath)
{
	std::ofstream outFile(filePath);
	if (not outFile.is_open()) {
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return;
	}

	for (const auto& gun : gun_table | std::views::values) {
		outFile << gun << std::endl;
	}

	outFile.close();

	return;
}
