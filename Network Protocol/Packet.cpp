#include "pch.h"
#include "Packet.h"


namespace myNP
{
	void BASE_PACKET::ntohByteOrder()
	{

	}

	CS_MATCHMAKING_PACKET CS_MATCHMAKING_PACKET::MakePacket()
	{
		return CS_MATCHMAKING_PACKET();
	}

	void CS_MATCHMAKING_PACKET::ntohByteOrder()
	{

	}

	CS_MOVE_PACKET CS_MOVE_PACKET::MakePacket(uint32_t player_id, float x, float y, bool direction)
	{
		return CS_MOVE_PACKET(player_id, x, y, direction);
	}

	void CS_MOVE_PACKET::ntohByteOrder()
	{
		//p_id = ntohl(p_id);
		//posX = ntohf(posX);
		//posY = ntohf(posY);
	}

	CS_FIRE_PACKET CS_FIRE_PACKET::MakePacket(uint32_t bullet_id, float x, float y, bool direction, uint32_t bullet_type, std::chrono::milliseconds fire_time)
	{
		return CS_FIRE_PACKET(htonl(bullet_id), htonf(x), htonf(y), direction, htonl(bullet_type), std::chrono::milliseconds(htonll(fire_time.count())));
	}

	void CS_FIRE_PACKET::ntohByteOrder()
	{
		b_id = ntohl(b_id);
		posX = ntohf(posX);
		posY = ntohf(posY);
		type = ntohl(type);
		fire_t = std::chrono::milliseconds(ntohll(fire_t.count()));
	}

	SC_MATCHMAKING_PACKET SC_MATCHMAKING_PACKET::MakePacket(bool success, uint32_t player_id)
	{
		return SC_MATCHMAKING_PACKET(success, player_id);
	}

	void SC_MATCHMAKING_PACKET::ntohByteOrder()
	{
		// p_id = ntohl(p_id);
	}

	SC_MOVE_PACKET SC_MOVE_PACKET::MakePacket(uint32_t player_id, float x, float y, bool direction)
	{
		return SC_MOVE_PACKET(player_id, x, y, direction);
	}

	void SC_MOVE_PACKET::ntohByteOrder()
	{
		//p_id = ntohl(p_id);
		//posX = ntohf(posX);
		//posY = ntohf(posY);
	}

	SC_PLAYER_DAMAGE_PACKET SC_PLAYER_DAMAGE_PACKET::MakePacket(float damaged)
	{
		return SC_PLAYER_DAMAGE_PACKET(htonf(damaged));
	}

	void SC_PLAYER_DAMAGE_PACKET::ntohByteOrder()
	{
		damage = ntohf(damage);
	}

	SC_FIRE_PACKET SC_FIRE_PACKET::MakePacket(uint32_t bullet_id, float x, float y, bool direction, uint32_t bullet_type, std::chrono::milliseconds fire_time)
	{
		return SC_FIRE_PACKET(htonl(bullet_id), htonf(x), htonf(y), direction, htonl(bullet_type), std::chrono::milliseconds(htonll(fire_time.count())));
	}

	void SC_FIRE_PACKET::ntohByteOrder()
	{
		b_id = ntohl(b_id);
		posX = ntohf(posX);
		posY = ntohf(posY);
		type = ntohl(type);
		fire_t = std::chrono::milliseconds(ntohll(fire_t.count()));
	}

	SC_BULLET_REMOVE_PACKET SC_BULLET_REMOVE_PACKET::MakePacket(uint32_t player_id, uint32_t bullet_id)
	{
		return SC_BULLET_REMOVE_PACKET(htonl(player_id), htonl(bullet_id));
	}

	void SC_BULLET_REMOVE_PACKET::ntohByteOrder()
	{
		p_id = ntohl(p_id);
		b_id = ntohl(b_id);
	}

	SC_ITEM_CREATE_PACKET SC_ITEM_CREATE_PACKET::MakePacket(uint32_t item_id, float x, float y)
	{
		return SC_ITEM_CREATE_PACKET(htonl(item_id), htonf(x), htonf(y));
	}

	void SC_ITEM_CREATE_PACKET::ntohByteOrder()
	{
		i_id = ntohl(i_id);
		posX = ntohf(posX);
		posY = ntohf(posY);
	}

	SC_ITEM_REMOVE_PACKET SC_ITEM_REMOVE_PACKET::MakePacket(uint32_t item_id)
	{
		return SC_ITEM_REMOVE_PACKET(htonl(item_id));
	}

	void SC_ITEM_REMOVE_PACKET::ntohByteOrder()
	{
		i_id = ntohl(i_id);
	}

	SC_GUN_UPDATE_PACKET SC_GUN_UPDATE_PACKET::MakePacket(uint32_t player_id, uint32_t gun_id)
	{
		return SC_GUN_UPDATE_PACKET(htonl(player_id), htonl(gun_id));
	}

	void SC_GUN_UPDATE_PACKET::ntohByteOrder()
	{
		p_id = ntohl(p_id);
		g_id = ntohl(g_id);
	}

	SC_LIFE_UPDATE_PACKET SC_LIFE_UPDATE_PACKET::MakePacket(uint32_t player_id)
	{
		return SC_LIFE_UPDATE_PACKET(htonl(player_id));
	}

	void SC_LIFE_UPDATE_PACKET::ntohByteOrder()
	{
		p_id = ntohl(p_id);
	}

	SC_GAMEOVER_PACKET SC_GAMEOVER_PACKET::MakePacket()
	{
		return SC_GAMEOVER_PACKET();
	}

	void SC_GAMEOVER_PACKET::ntohByteOrder()
	{

	}
}
