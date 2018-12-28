#pragma once
#include <iostream>
#include "Environment.h"

class BlockDispenser : public Block
{
private:
	double calcAngle(Point p1, Point p2);

public:
	string getImage(int x, int y)
	{
		return "images/Dispenser-Block.png";
	}

	void update(int x, int y);
};