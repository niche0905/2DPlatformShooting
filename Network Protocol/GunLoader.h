#pragma once
#include <unordered_map>

const char GunSavePath[] = "../Saved/Guns/GunAttribute.txt";


struct Gun
{
	std::string name{};	// ���� �̸�
	float damage{};		// ���� ������ (�°� ���ư��� �� ���ϱ� ����)
	float speed{};		// �Ѿ��� �ӵ� (�󸶳� ���� ���ư�����)
	int mag{};			// ź�� �󸶳� ���Ҵ���
	int RPM{};			// �д� �߻� �ӵ�
	int ratio{};		// ���� ���� Ȯ��(Ŭ���� ����) 0�̸� �ƿ� �ȳ���
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

