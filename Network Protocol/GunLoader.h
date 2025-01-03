#pragma once
#include <unordered_map>

const char GunSavePath[] = "../Saved/Guns/GunAttribute.txt";


struct Gun
{
	std::string name{};	// 총의 이름
	float damage{};		// 총의 데미지 (맞고 날아가는 힘 정하기 위한)
	float speed{};		// 총알의 속도 (얼마나 빨리 날아가는지)
	int mag{};			// 탄이 얼마나 남았는지
	int RPM{};			// 분당 발사 속도
	int ratio{};		// 총이 나올 확률(클수록 높다) 0이면 아예 안나옴
	int id{};

	Gun() = default;

	Gun(std::string name, float damage, float speed, int curMag, int RPM, int proMag)
		: name(name), damage(damage), speed(speed), mag(curMag), RPM(RPM), ratio(proMag) {}

	friend std::ostream& operator<<(std::ostream& os, const Gun& gun);
	friend std::istream& operator>>(std::istream& os, Gun& gun);
};

class GunLoader
{
private:
	std::unordered_map<uint8_t, Gun> gun_table;

public:
	GunLoader();

	GunLoader(const GunLoader&) = delete;
	GunLoader& operator=(const GunLoader&) = delete;
	GunLoader(GunLoader&&) = delete;
	GunLoader& operator=(GunLoader&&) = delete;
	
public:
	static GunLoader& Instance();

	const int getRandomGunId();

	bool loadGunFromFile(const std::string& filePath);

	void saveGunFromFile(const std::string& filePath);

	const std::unordered_map<uint8_t, Gun>& GetGunTable() const;
	std::unordered_map<uint8_t, Gun>& GetGunTable();
};

