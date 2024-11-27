#include "pch.h"
#include "NetworkUtils.h"

namespace myNP {
	void printPacketType(uint8_t packet_id)
	{
		using namespace myNP;
		using std::cout;
		using std::endl;
		PacketID id = static_cast<PacketID>(packet_id);
		switch (id)
		{
		case myNP::CS_MATCHMAKING:
			cout << "PACKET NAME: CS_MATCHMAKING_PACKET" << endl;
			break;
		case myNP::CS_MOVE:
			cout << "PACKET NAME: CS_MOVE_PACKET" << endl;
			break;
		case myNP::CS_FIRE:
			cout << "PACKET NAME: CS_FIRE_PACKET" << endl;
			break;
		case myNP::SC_MATCHMAKING:
			cout << "PACKET NAME: SC_MATCHMAKING_PACKET" << endl;
			break;
		case myNP::SC_MY_MOVE:
			cout << "PACKET NAME: SC_MOVE_PACKET" << endl;
			break;
		case myNP::SC_PLAYER_DAMAGE:
			cout << "PACKET NAME: SC_PLAYER_DAMAGE_PACKET" << endl;
			break;
		case myNP::SC_FIRE:
			cout << "PACKET NAME: SC_FIRE_PACKET" << endl;
			break;
		case myNP::SC_BULLET_REMOVE:
			cout << "PACKET NAME: SC_BULLET_REMOVE_PACKET" << endl;
			break;
		case myNP::SC_ITEM_CREATE:
			cout << "PACKET NAME: SC_ITEM_CREATE_PACKET" << endl;
			break;
		case myNP::SC_ITEM_REMOVE:
			cout << "PACKET NAME: SC_ITEM_REMOVE_PACKET" << endl;
			break;
		case myNP::SC_GUN_UPDATE:
			cout << "PACKET NAME: SC_GUN_UPDATE_PACKET" << endl;
			break;
		case myNP::SC_LIFE_UPDATE:
			cout << "PACKET NAME: SC_LIFE_UPDATE_PACKET" << endl;
			break;
		case myNP::SC_GAMEOVER:
			cout << "PACKET NAME: SC_GAMEOVER_PACKET" << endl;
			break;
		case myNP::END:
			break;
		default:
			break;
		}
	}

}