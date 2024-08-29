#pragma once
#include <string>
#include <fstream>


class Gun
{
private:
	std::string name;	// ���� �̸�
	float damage;		// ���� ������ (�°� ���ư��� �� ���ϱ� ����)
	float speed;		// �Ѿ��� �ӵ� (�󸶳� ���� ���ư�����)
	int curMag;			// ź�� �󸶳� ���Ҵ���
	int RPM;			// �д� �߻� �ӵ�
	int proMag;			// ���� ���� Ȯ��(Ŭ���� ����) 0�̸� �ƿ� �ȳ���

public:
	Gun(float damage, float speed, int curMag, int RPM) : damage(damage), speed(speed), curMag(curMag), RPM(RPM)
	{

	}

	friend std::ostream& operator<<(std::ostream& os, const Gun& gun);
	friend std::istream& operator>>(std::istream& os, Gun& gun);

};

