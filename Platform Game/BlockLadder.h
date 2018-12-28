#pragma once
#include <iostream>
#include "Environment.h"

class BlockLadder : public Block
{
public:
	string getImage(int x, int y)
	{
		return "images/Ladder-Block.png";
	}

	bool isSolid()
	{
		return false;
	}
};