#include "pch.h"
#include "Player.h"


Player::Player(bool dir) 
	: directon(dir)
	, life(BaseLife)
{

}

Player::Player(float pos_x, float pos_y, bool dir)
	: Object(pos_x, pos_y, myNP::PlayerWidth, myNP::PlayerHeight, 0.5f, 1.0f)
	, directon(dir)
	, life(BaseLife)
{

}
