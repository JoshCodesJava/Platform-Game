#pragma once
#include "Environment.h"
#include "Utility.h"

class EntityCharacter : public Entity
{
private:
	bool falling;
	int fallingSpeed;
	int fallingFrame;
	int jumpingLength;
	int jumpingFrame;
	bool jumping;
	int jumpingCountFrame;
	int jumpingCountSpeed;
	bool facingLeft;
	bool facingRight;
	static const int keyLeft = sf::Keyboard::Left;
	static const int keyRight = sf::Keyboard::Right;
	static const int keyJump = sf::Keyboard::Space;
	bool right;
	bool left;
	int movementSpeed;
	int movementFrame;
	int climbingSpeed;
	int climbingFrame;
	int rightIndex;
	int leftIndex;
	int jumpingHighLength;
	int jumpingHighFrame;
	bool jumpingHigh;
	int jumpingHighCountFrame;
	bool up;
	int update_int;
	bool init_bool;
	int retries;
	Point respawnPoint;
	vector<Entity*> oldEntities;
	int oldXs;
	int score;

public:
	EntityCharacter(World* world) : Entity(0, 0, world), respawnPoint(0, 0)
	{
		falling = true;
		fallingSpeed = 0;
		fallingFrame = 0;
		jumpingLength = 200;
		jumpingFrame = 0;
		jumping = false;
		jumpingCountFrame = 0;
		jumpingCountSpeed = fallingSpeed;
		facingLeft = false;
		facingRight = true;
		right = false;
		left = false;
		movementSpeed = 1;
		movementFrame = 0;
		climbingSpeed = 2;
		climbingFrame = 0;
		rightIndex = 0;
		leftIndex = 0;
		jumpingHighLength = 400;
		jumpingHighFrame = 0;
		jumpingHigh = false;
		jumpingHighCountFrame = 0;
		up = false;
		update_int = 0;
		init_bool = false;
		retries = 2;
		oldXs = 0;
		score = 0;
	}

	void update(bool inRange) 
	{		
		if(getScore()>=100)
		{
			retries++;
			score-=100;
		}

		if(!init_bool)
		{
			save();
			init_bool = true;
		}

		update_int++;

		if(update_int == 40)
			update_int = 0;

		if(getY()>1000)
		{
			die();
			return;
		}

		boolean climbing = onLadder()&&up;

		if(climbing && canJump())
		{
			if(climbingFrame <= climbingSpeed)
			{
				setY(getY() - 1);
				climbingFrame = 0;
			}
			else
			{
				climbingFrame++;
			}
		}

		if(!jumping&&!jumpingHigh&&!climbing)
		{
			if(fallingFrame <= fallingSpeed)
			{
				falling = canFall();

				if(falling)
				{
					setY(getY() + 1);
				}

				fallingFrame = 0;
			}
			else
			{
				fallingFrame+=1;
			}
		}

		if(jumpingCountFrame >= jumpingCountSpeed)
		{
			if(jumping)
			{
				if(jumpingFrame <= jumpingLength)
				{
					if(canJump())	
					{
						setY(getY() - 1);
					}
					else
						jumping = false;

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

		if(jumpingHighCountFrame >= 0)
		{
			if(jumpingHigh)
			{
				if(jumpingHighFrame <= jumpingHighLength)
				{
					if(canJump())	
					{
						setY(getY() - 1);
					}
					else
						jumpingHigh = false;

					jumpingHighFrame+=1;
				}
				else
				{
					jumpingHighFrame=0;
					jumpingHigh = false;
				}
			}

			jumpingHighCountFrame=0;
		}
		else
		{
			jumpingHighCountFrame+=1;
		}

		if(movementFrame>=movementSpeed)
		{
			if(right)
			{	
				if(getX()+getWidth()<(float)(X_BLOCKS*BLOCKSIZE))
				{
					if(canMoveRight())
					{
						if(update_int==0)
							rightIndex++;

						if(getX()+getWidth()<X_BLOCKS*BLOCKSIZE)
							setX(getX() + 1);

						if((float)(getX()*SCALE)>(float)((WIDTH/2)-(getWidth()*SCALE/2))&&(float)(theWorld->getXs()+WIDTH)<(float)(X_BLOCKS*BLOCKSIZE*SCALE))
						{
							theWorld->setXs(theWorld->getXs() + 1);
						}
					}
				}
			}

			if(left)
			{
				if(getX()>0)
				{
					if(canMoveLeft())
					{
						if(update_int==0)
							leftIndex++;

						if(getX()>0)
							setX(getX() - 1);

						if((float) (getX()*SCALE) < (float)(X_BLOCKS*BLOCKSIZE*SCALE-(WIDTH/2)) && (float)theWorld->getXs()*SCALE>0)
							theWorld->setXs(theWorld->getXs() - 1);
					}
				}
			}

			movementFrame = 0;
		}
		else
		{
			movementFrame+=1;
		}

		if(!left&&facingLeft)
			leftIndex = 0;

		if(!right&&facingRight)
			rightIndex = 0;
	}

private:
	bool canMoveRight()
	{
		for(int x = (int) getX(); x<getX()+getWidth()+1; x++)
		{
			for(int y = (int) getY(); y<getY()+getHeight(); y++)
			{
				Block* check = theWorld->getBlock((int) floor(x/BLOCKSIZE), (int) floor(y/BLOCKSIZE));

				if(check != NULL && check->isSolid())
				{
					return false;
				}
			}
		}

		return true;
	}

	bool canMoveLeft()
	{
		for(int x = (int) getX()-1; x<getX()+getWidth(); x++)
		{
			for(int y = (int) getY(); y<getY()+getHeight(); y++)
			{
				Block* check = theWorld->getBlock((int) floor(x/BLOCKSIZE), (int) floor(y/BLOCKSIZE));

				if(check != NULL && check->isSolid())
				{
					return false;
				}
			}
		}

		return true;
	}

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

		return true;
	}

	bool canJump()
	{
		for(int x = (int) getX(); x<getX()+getWidth(); x++)
		{
			for(int y = (int) getY()-1; y<getY()+getHeight(); y++)
			{
				Block* check = theWorld->getBlock((int) floor(x/BLOCKSIZE), (int)floor(y/BLOCKSIZE));

				if(check != NULL && check->isSolid())
				{
					return false;
				}
			}
		}

		return true;
	}

public:
	string getImage()
	{
		if(facingRight)
		{
			switch(rightIndex%4)
			{
			case 0 : return "images/Player_Right_1.png";
			case 1 : return "images/Player_Right_2.png";
			case 2 : return "images/Player_Right_3.png";
			case 3 : return "images/Player_Right_4.png";
			default : return NULL;
			}
		}
		else if(facingLeft)
		{
			switch(leftIndex%4)
			{
			case 0 : return "images/Player_Left_1.png";
			case 1 : return "images/Player_Left_2.png";
			case 2 : return "images/Player_Left_3.png";
			case 3 : return "images/Player_Left_4.png";
			default : return NULL;
			}
		}
		else
		{
			return "images/Player_Left_1.png";
		}
	}

	void keyPressed(int e) 
	{
		if(e==keyLeft)
		{
			facingRight = false;
			facingLeft = true;
			right = false;
			left = true;
		}

		else if(e==keyRight)
		{
			facingLeft = false;
			facingRight = true;
			left = false;
			right = true;
		}

		else if(!canFall() && e==keyJump)
		{			
			jumping = true;
			theWorld->playSound("sound/jump.wav");
		}

		else if(e==sf::Keyboard::Up)
		{
			up = true;
		}
	}

	void keyReleased(int e)
	{
		if(e==keyLeft)
		{
			left = false;
		}

		if(e==keyRight)
		{
			right = false;
		}

		if(e==sf::Keyboard::Up)
		{
			up = false;
		}
	}

	void die();

	int getTextureWidth()
	{
		return 21;
	}

	int getTextureHeight()
	{
		return 43;
	}

	int getWidth() 
	{
		return (int) (1.5 * 21);
	}

	int getHeight() 
	{
		return (int) (1.5 * 43);
	}

	void jump() 
	{
		jumpingHigh = true;
	}

private:

	bool onLadder()
	{				
		if(jumping || jumpingHigh)
			return false;

		bool ladder = false;

		for(int x = getX(); x<getX()+getWidth(); x++)
		{
			for(int y = getY(); y<getY()+getHeight(); y++)
			{
				Block* check = theWorld->getBlock((int) floor(x/BLOCKSIZE), (int)floor(y/BLOCKSIZE));

				if(check != NULL)
				{
					if(check->isSolid())
					{
						return false;
					}
				}

				if(check == theWorld->ladder)
				{
					ladder = true;
				}
			}
		}

		if(ladder == true)
			return true;

		else
		{			
			for(int x = getX(); x<getX()+getWidth(); x++)
			{
				for(int y = getY(); y<getY()+getHeight()+1; y++)
				{
					Block* check = theWorld->getBlock((int) floor(x/BLOCKSIZE), (int)floor(y/BLOCKSIZE));

					if(check != NULL)
					{
						if(check->isSolid())
						{
							return false;
						}
					}

					if(check == theWorld->ladder)
					{
						ladder = true;
					}
				}
			}
		}

		return ladder;
	}

public:
	void EntityCharacter::bordersEntity(Direction dir, Entity* check);

	void win() 
	{
		theWorld->playSound("sound/win.wav");
		theWorld->terminate();	
	}

	int getRetries()
	{
		return retries;
	}

	void save()
	{
		oldEntities.clear();

		for(unsigned int i = 0; i < theWorld->getEntities().size(); i++)
		{
			Entity* entity = theWorld->getEntities()[i];

			if(!(entity == this) && !entity->impermanent())
			{
				oldEntities.push_back(entity->clone());
			}
		}

		respawnPoint.setLocation(getX(), getY());
		oldXs = theWorld->getXs();
	}

	void addToScore(int num) 
	{
		score+=num;
	}

	int getScore() 
	{
		return score;
	}

	EntityCharacter* clone()
	{
		throw "ERROR";
		return (EntityCharacter*) 0;
	}
};