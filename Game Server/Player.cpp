#include "pch.h"
#include "Player.h"


Player::Player(bool dir) 
	: directon(dir)
	, life(BaseLife)
	, gunId(BaseGunID)
	, curMag(-1)
{

}
