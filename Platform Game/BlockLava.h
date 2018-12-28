#pragma once
#include <iostream>
#include "Environment.h"

class BlockLava : public Block
{
public:
	string getImage(int x, int y)
	{
		return "images/Lava-Block.png";
	}

	void intersectsWithEntity(Entity *entity, int x, int y);

	bool isSolid()
	{
		return false;
	}
};