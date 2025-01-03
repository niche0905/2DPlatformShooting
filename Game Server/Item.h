#pragma once
#include "MovingObject.h"
#include "Level.h"

constexpr float itemSpawnHeight{ -800.0f };


class Item : public MovingObject
{
private:
	static uint32_t nextID;

private:
	uint32_t itemID;
	bool OnAir;

	Level* level;

public:
	Item() = default;

	Item(float x, float y, Level* level);

	void Update(int64_t delta_time);

	uint32_t GetItemID() const;

};

