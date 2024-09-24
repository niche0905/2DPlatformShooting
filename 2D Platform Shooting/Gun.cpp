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

std::random_device rd;
std::default_random_engine gen(rd());

const int getRandomGunId()
{
	int total{};

	// ratio를 모두 더한다.
	for (const auto ratio : g_guns
		| std::views::values
		| std::views::transform(&Gun::getRatio)) {
		total += ratio > 0 ? ratio : 0;
	}

	// 난수를 생성한다.
	std::uniform_int_distribution dist(1, total);
	int random_value{ dist(gen) };
	int cnt{};
	for (const auto ratio : g_guns
		| std::views::values
		| std::views::transform(&Gun::getRatio)) {
		random_value -= ratio > 0 ? ratio : 0;
		if (random_value <= 0) { break; }
		++cnt;
	}

	std::cout << cnt << '\n';
	return cnt;
}
