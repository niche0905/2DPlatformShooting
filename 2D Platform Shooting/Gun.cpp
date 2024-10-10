#include "pch.h"
#include "Gun.h"
// [cham] 9.22: 
std::unordered_map<uint8_t, Gun> g_guns;

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

const int getRandomGunId()
{
	int total{};

	// ratio를 모두 더한다.
	std::ranges::for_each(g_guns | std::views::values, [&total](const auto& gun) {
		total += gun.ratio > 0 ? gun.ratio : 0;
		});

	// 난수를 생성한다.
	std::uniform_int_distribution dist(1, total);
	int random_value{ dist(RANDOM_ENGINE) };

	// ratio에 따라 값을 반환한다.
	int cnt{};
	for (const auto& gun : g_guns | std::views::values) {
		random_value -= gun.ratio > 0 ? gun.ratio : 0;
		if (random_value <= 0) { break; }
		++cnt;
	}

	std::println("{}", cnt);
	return cnt;
}
