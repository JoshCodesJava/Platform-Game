#include "EntityCharacter.h"
#include "EntityDiamond.h"
#include "EntitySlime.h"
#include "EntityWoompa.h"

void EntityCharacter::die()
{
	falling = true;
	fallingFrame = 0;
	jumpingFrame = 0;
	jumping = false;
	jumpingCountFrame = 0;
	facingLeft = false;
	facingRight = true;
	right = false;
	left = false;
	movementFrame = 0;
	climbingFrame = 0;
	rightIndex = 0;
	leftIndex = 0;
	jumpingHighFrame = 0;
	jumpingHigh = false;
	jumpingHighCountFrame = 0;
	update_int = 0;

	theWorld->abort();

	if(retries>0)
	{		
		theWorld->playSound("sound/die.wav");
		retries--;

		for(unsigned int i = 0; i < theWorld->getEntities().size(); i++)
		{
			if(theWorld->getEntities()[i] != this)
				delete theWorld->getEntities()[i];
		}

		theWorld->clear();
		theWorld->addEntity(this);

		for(unsigned int i = 0; i < oldEntities.size(); i++)
		{
			Entity* entity = oldEntities[i];
			theWorld->addEntity(entity);
		}

		if(score>0)
		{
			EntityDiamond* diamond = new EntityDiamond(getX(),getY(),theWorld,score);
			theWorld->addEntity(diamond);
		}

		score = 0;
		theWorld->setXs(oldXs);
		setX(respawnPoint.getX());
		setY(respawnPoint.getY());
		save();
	}
	else
	{
		theWorld->playSound("sound/death.wav");
		theWorld->terminate();	
	}
}

void EntityCharacter::bordersEntity(Direction dir, Entity* check) 
{
	if(typeid(*check) == typeid(EntitySlime))
	{
		if(((EntitySlime*)check)->invincible())
		{
			die();
		}
		else
		{
			switch(dir)
			{
			case DOWN : 
				theWorld->playSound("sound/splat.wav");
				check->setForDeletion();
				score++; 
				break;
			default : 
				die();
			}
		}
	}
	else if(typeid(*check) == typeid(EntityWoompa))
	{
		switch(dir)
		{
		case DOWN : 
			theWorld->playSound("sound/splat.wav");
			check->setForDeletion();
			score++; 
			break;
		default : 
			die();
		}
	}
}