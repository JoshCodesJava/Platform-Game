#pragma once
#include <iostream>
#include "Environment.h"

class BlockDiamond : public Block
{
public:
	string getImage(int x, int y)
	{
		return "images/Star.png";
	}

	bool isSolid()
	{
		return false;
	}

	void intersectsWithEntity(Entity *entity, int x, int y);
};