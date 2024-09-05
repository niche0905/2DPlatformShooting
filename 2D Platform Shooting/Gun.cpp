#include "Gun.h"


bool loadGunFromFile(const std::string& filePath, std::vector<Gun>& guns)
{
	std::ifstream inFile(filePath);
	if (not inFile.is_open()) {
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return false;
	}

	Gun gun;
	while (inFile >> gun)
	{
		guns.push_back(gun);
	}

	inFile.close();

	return true;
}

void saveGunFromFile(const std::string& filePath, const std::vector<Gun>& guns)
{
	std::ofstream outFile(filePath);
	if (not outFile.is_open()) {
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return;
	}

	for (const Gun& gun : guns) {
		outFile << gun << std::endl;
	}

	outFile.close();

	return;
}

std::ostream& operator<<(std::ostream& os, const Gun& gun)
{
	os << "[ " << gun.name << " ]" << std::endl;
	os << "Damage: " << gun.damage << std::endl;
	os << "Speed: " << gun.speed << std::endl;
	os << "Mag: " << gun.curMag << std::endl;
	os << "RPM: " << gun.RPM << std::endl;
	os << "Ratio: " << gun.proMag << std::endl;

	return os;
}

std::istream& operator>>(std::istream& is, Gun& gun)
{
	std::string temp;
	is >> temp >> gun.name >> temp;
	is >> temp >> gun.damage;
	is >> temp >> gun.speed;
	is >> temp >> gun.curMag;
	is >> temp >> gun.RPM;
	is >> temp >> gun.proMag;

	return is;
}

void Gun::draw(sf::RenderWindow& window)
{
	for (const Bullet& bullet : bullets) {
		bullet.draw(window);
	}
}

void Gun::firebullet(bool direction, sf::Vector2f position)
{
	bullets.push_back(Bullet(direction, position));
}

void Gun::updateBullets(long long deltaTime)
{
	for (auto it = bullets.begin(); it != bullets.end(); ) {
		it->update(deltaTime);
		// 총알이 레벨 밖으로 나갈 때 지우게 하는 코드
		// 초기에는 Player가 level을 가지고 있었기 때문에 레벨의 크기에 접근이 가능했으나
		// 총알을 Player가 아닌 Gun으로 빼는 과정에서 레벨 크기에 접근이 불가능 해졌으므로
		// 임시로 지우지 않도록 주석 처리를 함(민경원 9/4)
		/*
		if (it->isOutBounds(level.leftBound - 1000.0f, level.rightBound + 1000.0f))
			it = bullets.erase(it);
		else
			++it;
		*/
		++it; // 위의 문제 해결하게 되면 이건 지우기
	}
}