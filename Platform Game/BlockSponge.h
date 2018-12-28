#pragma once
#include <iostream>
#include "Environment.h"

class BlockSponge : public Block
{
public:
	string getImage(int x, int y)
	{
		return "images/Sponge-Block.png";
	}

	void bordersEntity(Direction dir, Entity *entity, int x, int y);
};