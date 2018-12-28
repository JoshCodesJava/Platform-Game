#pragma once
#include "Environment.h"
#include "Utility.h"
#include <iostream>

class EntityFireball : public Entity
{
private:
	double angle;
	double speed;
	int life;
	double real_x;
	double real_y;

public:
	EntityFireball(int x, int y, double angle, double speed, World* world) : Entity(x, y, world)
	{
		this->angle = angle;
		this->speed = speed;
		life = 0;
		real_x = x;
		real_y = y;
	}

	void update(bool inRange) 
	{
		life++;

		if(life>2500)
			setForDeletion();

		if(inRange)
		{
			float xDirection = (float) (cos((float) toRadians(angle)) * speed);
			float yDirection = (float) (sin((float) toRadians(angle)) * speed);
			real_x = real_x + xDirection;
			real_y = real_y + yDirection;
			setX((int) real_x);
			setY((int) real_y);
		}

		int x1 = (int)floor((getX()+getWidth())/BLOCKSIZE);
		int x2 = (int)floor((getX())/BLOCKSIZE);
		int y1 = (int)floor(getY()/BLOCKSIZE);
		int y2 = (int)floor((getY()+getHeight()-1)/BLOCKSIZE);
		Block* block1 = theWorld->getBlock(x1, y1);
		Block* block2 = theWorld->getBlock(x1, y2);
		Block* block3 = theWorld->getBlock(x2, y1);
		Block* block4 = theWorld->getBlock(x2, y2);

		if(block1 != NULL && block1->isSolid() && !(block1 == theWorld->dispenser))
			setForDeletion();

		else if(block2 != NULL && block2->isSolid() && !(block2 == theWorld->dispenser))
			setForDeletion();

		else if(block3 != NULL && block3->isSolid() && !(block3 == theWorld->dispenser))
			setForDeletion();

		else if(block4 != NULL && block4->isSolid() && !(block4 == theWorld->dispenser))
			setForDeletion();
	}

	void bordersEntity(Direction dir, Entity* check);
	
	string getImage() 
	{
		return "images/Fireball.png";
	}

	EntityFireball* clone()
	{
		throw "ERROR";
	}

	bool impermanent()
	{
		return true;
	}

	int getWidth()
	{
		 return BLOCKSIZE/2;
	}

	int getHeight()
	{
		return BLOCKSIZE/2;
	}

	int getTextureWidth()
	{
		return 20;
	}

	int getTextureHeight()
	{
		return 20;
	}
};