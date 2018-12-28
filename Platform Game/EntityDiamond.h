#pragma once
#include "Environment.h"
#include "Utility.h"

class EntityDiamond : public Entity
{
private:
	int stackSize;

	bool canFall()
	{
		for(int x = (int) getX(); x<getX()+getWidth(); x++)
		{
			for(int y = (int) getY(); y<getY()+getHeight()+1; y++)
			{
				Block* check = theWorld->getBlock((int) floor(x/BLOCKSIZE), (int)floor(y/BLOCKSIZE));

				if(check != NULL && check->isSolid())
				{
					return false;
				}
			}
		}

		return true && getY() < Y_BLOCKS * BLOCKSIZE;
	}

public:
	EntityDiamond(int x, int y, World* world, int stackSize) : Entity(x, y, world)
	{
		this->stackSize = stackSize;
		
		while(canFall())
			EntityDiamond::setY(getY()+1);
	}

	void update(bool inRange) 
	{		
		if(getY() > Y_BLOCKS * BLOCKSIZE)
		{
			setForDeletion();
		}
	}

	string getImage()
	{
		return "images/Star.png";
	}

	int getWidth()
	{
		return BLOCKSIZE;
	}

	int getHeight()
	{
		return BLOCKSIZE;
	}

	int getTextureWidth()
	{
		return 40;
	}

	int getTextureHeight()
	{
		return 40;
	}

	void bordersEntity(Direction dir, Entity* check)
	{
		if(typeid(*check) == typeid (EntityCharacter))
		{
			((EntityCharacter*) check)->addToScore(stackSize);
			this->setForDeletion();
		}
	}

	EntityDiamond* clone()
	{
		throw "ERROR";
	}

	bool impermanent()
	{
		return true;
	}
};