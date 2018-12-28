#pragma once
#include <iostream>
#include "Environment.h"

class BlockGround : public Block
{
public:
	string getImage(int x, int y)
	{
		return "images/Ground-Block.png";
	}
};