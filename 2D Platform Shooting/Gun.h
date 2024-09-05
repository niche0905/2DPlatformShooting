#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include "Bullet.h"

const std::string GunSavePath = "./Saved/Guns/GunAttribute.txt";

bool loadGunFromFile(const std::string& filePath, std::vector<class Gun>& guns);
void saveGunFromFile(const std::string& filePath, const std::vector<class Gun>& guns);


class Gun
{
private:
	std::string name;	// ���� �̸�
	std::list<Bullet> bullets;
	float damage;		// ���� ������ (�°� ���ư��� �� ���ϱ� ����)
	float speed;		// �Ѿ��� �ӵ� (�󸶳� ���� ���ư�����)
	int curMag;			// ź�� �󸶳� ���Ҵ���
	int RPM;			// �д� �߻� �ӵ�
	int proMag;			// ���� ���� Ȯ��(Ŭ���� ����) 0�̸� �ƿ� �ȳ���

public:
	Gun()
	{

	}
	Gun(std::string name, float damage, float speed, int curMag, int RPM, int proMag) : name(name), damage(damage), speed(speed), curMag(curMag), RPM(RPM), proMag(proMag)
	{

	}

	friend std::ostream& operator<<(std::ostream& os, const Gun& gun);
	friend std::istream& operator>>(std::istream& os, Gun& gun);

	void draw(sf::RenderWindow& window);
	void firebullet(bool direction, sf::Vector2f position);
	void updateBullets(long long deltaTime);
};

