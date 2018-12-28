#pragma once
#include <iostream>
#include "Environment.h"

class BlockPortal : public Block
{
public:
	string getImage(int x, int y)
	{
		return "images/Portal-Block.png";
	}

	void intersectsWithEntity(Entity *entity, int x, int y);

	bool isSolid()
	{
		return false;
	}
};