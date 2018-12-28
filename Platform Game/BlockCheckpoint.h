#pragma once
#include <iostream>
#include "Environment.h"

class BlockCheckpoint : public Block
{
public:
	string getImage(int x, int y) 
	{
		if(theWorld->getMetaData(x,y) == "activated")
			return "images/Checkpoint 1.png";
		else
			return "images/Checkpoint 2.png";
	}

	void BlockCheckpoint::intersectsWithEntity(Entity* ent, int x, int y);

	bool isSolid()
	{
		return false;
	}
};