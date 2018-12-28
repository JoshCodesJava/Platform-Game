#pragma once
#include "Environment.h"
#include "Utility.h"

class EntitySlime : public Entity
{
private:
	bool facingLeft;
	bool facingRight;
	bool jumping;
	int movementFrame;
	int jumpingFrame;
	bool falling;
	int fallingFrame;
	int jumpingCountFrame;
	int count;

public:
	EntitySlime(int x, int y, World* world) : Entity(x, y, world)
	{
		facingLeft = true;
		facingRight = false;
		jumping = false;
		movementFrame = 0;
		jumpingFrame = 0;
		falling = true;
		fallingFrame = 0;
		jumpingCountFrame = 0;
		count = 0;
	}

	void EntitySlime::update(bool inRange) 
	{
		if(inRange)
		{
			if(getY()>1000)
				setForDeletion();

			if(!jumping)
			{
				Point pt1(getX(), getY() + getHeight());
				Point pt2(getX()+getWidth(), getY() + getHeight());
				int x1 = (int) floor(getX()/BLOCKSIZE);
				int x2 = (int) floor((getX()+getWidth())/BLOCKSIZE);
				int y = (int) floor((getY()+getHeight())/BLOCKSIZE);
				ContainRectangle rect1(pt1.getX(), pt1.getY(), BLOCKSIZE, BLOCKSIZE);
				ContainRectangle rect2(pt2.getX(), pt2.getY(), BLOCKSIZE, BLOCKSIZE);

				if(fallingFrame <= 0)
				{
					if(rect1.contains(pt1) && (theWorld->getBlock(x1, y)!=NULL && theWorld->getBlock(x1, y)->isSolid()))
					{
						falling = false;
					}
					else if(rect2.contains(pt2) && theWorld->getBlock(x2, y)!=NULL && theWorld->getBlock(x2, y)->isSolid())
					{
						falling = false;
					}
					else
					{
						falling = true;
					}

					if(falling)
					{
						setY(getY() + 1);
					}
					else
					{
						jumping = true;
						count++;
						count = count%4;
						theWorld->playSound("sound/slime.wav");
					}

					fallingFrame = 0;
				}
				else
				{
					fallingFrame+=1;
				}
			}

			if(jumpingCountFrame >= 2)
			{
				if(jumping)
				{
					if(jumpingFrame <= 100)
					{
						Block* check1 = theWorld->getBlock((int)floor(getX()/BLOCKSIZE), (int)floor((getY()-1)/BLOCKSIZE));
						Block* check2 = theWorld->getBlock((int)floor((getX()+getWidth())/BLOCKSIZE), (int)floor((getY()-1)/BLOCKSIZE));

						if((check1==NULL||!check1->isSolid()) && ((check2==NULL)||!check2->isSolid()))						
							setY(getY() - 1);
						jumpingFrame+=1;
					}
					else
					{
						jumpingFrame=0;
						jumping = false;
					}
				}

				jumpingCountFrame=0;
			}
			else
			{
				jumpingCountFrame+=1;
			}

			if(movementFrame >= (falling||jumping ? 4 : 1))
			{
				if(facingLeft)
				{
					int x = (int) floor((getX()-1)/BLOCKSIZE);
					int y1 = (int) floor(getY()/BLOCKSIZE);
					int y2 = (int) floor((getY()+getHeight()-1)/BLOCKSIZE);
					Block* check1 = theWorld->getBlock(x, y1);
					Block* check2 = theWorld->getBlock(x, y2);
					boolean canMove = false;

					if((check1==NULL||!check1->isSolid())&&(check2==NULL||!check2->isSolid()))
						canMove = true;

					if(x <= 0)
					{
						canMove = false;
					}

					if(canMove)
					{
						setX(getX() - 1);
					}
					else
					{
						facingRight = true;
						facingLeft = false;
					}
				}

				if(facingRight)
				{
					int x = (int) floor((getX()+getWidth())/BLOCKSIZE);
					int y1 = (int) floor(getY()/BLOCKSIZE);
					int y2 = (int) floor((getY()+getHeight()-1)/BLOCKSIZE);
					Block* check1 = theWorld->getBlock(x, y1);
					Block* check2 = theWorld->getBlock(x, y2);
					boolean canMove = false;

					if((check1==NULL||!check1->isSolid())&&(check2==NULL||!check2->isSolid()))
						canMove = true;

					if(x >= BLOCKSIZE * X_BLOCKS)
					{
						canMove = false;
					}

					if(canMove)
					{
						setX(getX() + 1);
					}
					else
					{
						facingRight = false;
						facingLeft = true;
					}
				}

				movementFrame = 0;
			}
			else
			{
				movementFrame+=1;
			}
		}
	}

	string getImage()
	{
		if(facingLeft)
		{
			switch(count)
			{
			case 0:
				return "images/Slime_Left_1.png";
			case 1:
				return "images/Slime_Left_2.png";
			case 2:
				return "images/Slime_Left_3.png";
			case 3:
				return "images/Slime_Left_4.png";
			}
		}
		if(facingRight)
		{
			switch(count)
			{
			case 0:
				return "images/Slime_Right_1.png";
			case 1:
				return "images/Slime_Right_2.png";
			case 2:
				return "images/Slime_Right_3.png";
			case 3:
				return "images/Slime_Right_4.png";
			}
		}
	}

	int getWidth() 
	{
		return BLOCKSIZE+(BLOCKSIZE/4);
	}

	int getHeight() 
	{
		return BLOCKSIZE+(BLOCKSIZE/4);
	}

	int getTextureWidth() 
	{
		return 64;
	}

	int getTextureHeight() 
	{
		return 64;
	}

	static int getStartingWidth() 
	{
		return BLOCKSIZE+(BLOCKSIZE/4);
	}

	static int getStartingHeight() 
	{
		return BLOCKSIZE+(BLOCKSIZE/4);
	}

	bool invincible()
	{
		return count != 0;
	}

	EntitySlime* clone()
	{
		return new EntitySlime(getX(), getY(), theWorld);
	}
};