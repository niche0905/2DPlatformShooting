#include "Gun.h"


std::ostream& operator<<(std::ostream& os, const Gun& gun)
{
	os << "[" << gun.name << "]" << std::endl;
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