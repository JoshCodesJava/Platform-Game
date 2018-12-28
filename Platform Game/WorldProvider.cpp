#include "Environment.h"
#include "BlockGround.h"
#include "BlockSponge.h"
#include "BlockLava.h"
#include "BlockLadder.h"
#include "BlockPortal.h"
#include "BlockDiamond.h"
#include "BlockCheckpoint.h"
#include "BlockDispenser.h"
#include "EntityCharacter.h"
#include "EntitySlime.h"
#include "EntityWoompa.h"

int nextInt(int upto)
{
	return rand()%upto;
}

void World::loadLevel()
{
	enum CurrentDirection
	{
		FLAT, UP, DOWN, JUMPBOUNCE
	};

	CurrentDirection biomes[X_BLOCKS];
	int elevations[X_BLOCKS];
	int currentY = 18;
	int emptyTime = 0;
	int nearbyObject = 0;
	int nearbyMonster = 0;
	int dispenserLimit = 8;
	CurrentDirection direction = FLAT;

	for(int currentX = 0; currentX < X_BLOCKS; currentX++)
	{
		biomes[currentX] = direction;
		elevations[currentX] = currentY;

		if(dispenserLimit > 0)
			dispenserLimit--;

		if(nearbyObject > 0)
			nearbyObject--;

		if(nearbyMonster > 0)
			nearbyMonster--;

		if(direction == JUMPBOUNCE)
			emptyTime++;
		else
			emptyTime = 0;

		if(nextInt(7) == 0)
			setBlock(currentX, currentY-3, diamond);

		if(direction == JUMPBOUNCE)
		{
			setBlock(currentX, 18, lava);

			if(emptyTime % 20 == 2)
			{
				if(dispenserLimit == 0)
				{
					setBlock(currentX, currentY, dispenser);
					dispenserLimit+=6;
				}
			}

			if(emptyTime % 6 == 4)
			{
				setBlock(currentX, currentY, sponge);
			}
		}
		else
		{
			for(int elevation = currentY; elevation < 19; elevation++)
				setBlock(currentX, elevation, ground);
		}

		if(direction == FLAT && currentX > 5 && biomes[currentX - 1] == FLAT && biomes[currentX - 2] == FLAT && nextInt(15) == 0 && currentY > 15 && nearbyObject == 0)
		{
			for(int i = currentY - 10; i < currentY; i++)
			{
				setBlock(currentX, i, ground);

				if(i % 4 == 3)
				{
					setBlock(currentX - 1, i, ground);
				}
			}

			setBlock(currentX, currentY - 10, lava);
			elevations[currentX - 1] = currentY - 10;
			elevations[currentX] = currentY - 10;
			currentY-=10;
			nearbyObject = 13;
		}

		if(direction == FLAT && currentX > 5 && biomes[currentX - 1] == FLAT && biomes[currentX - 2] == FLAT && biomes[currentX - 3] == FLAT && nextInt(10) == 0 && currentY > 15 && nearbyObject == 0)
		{
			for(int i = currentY - 10; i < currentY; i++)
			{
				setBlock(currentX, i, ground);
				setBlock(currentX - 1, i, ladder);
			}

			if(dispenserLimit == 0)
			{
				setBlock(currentX, currentY - 10, dispenser);
				dispenserLimit+=6;
			}

			elevations[currentX] = currentY - 10;
			currentY-=10;
			nearbyObject = 13;
		}

		switch(direction)
		{			
		case FLAT:
			switch(nextInt(20))
			{
			case 0:
				direction = UP;
				break;
			case 1:
				direction = DOWN;
				break;
			case 2:
				if(nextInt(10) == 0)
					direction = JUMPBOUNCE;
				break;
			}

		case UP:
			switch(nextInt(10))
			{
			case 0:
				direction = FLAT;
				break;
			case 1:
				direction = DOWN;
				break;
			case 2:
				if(nextInt(10) == 0)
					direction = JUMPBOUNCE;
				break;
			}

		case DOWN:
			switch(nextInt(10))
			{
			case 0:
				direction = UP;
				break;
			case 1:
				direction = FLAT;
				break;
			case 2:
				if(nextInt(10) == 0)
					direction = JUMPBOUNCE;
				break;
			}

		case JUMPBOUNCE:
			switch(nextInt(45))
			{
			case 0:
				direction = UP;
				break;
			case 1:
				direction = DOWN;
				break;
			case 2:
				direction = FLAT;
				break;
			}

			break;
		}

		if(currentX % 200 == 0 && direction != JUMPBOUNCE)
			setBlock(currentX, currentY-1, checkpoint);

		switch(direction)
		{
		case UP:
			if(currentY > 5)
				currentY--;
			break;
		case DOWN:
			if(currentY < 18)
				currentY++;
			break;
		case JUMPBOUNCE:
			int option = rand()%3;

			switch(option)
			{
			case 0:
				if(currentY > 5)
					currentY--;
				break;

			case 1:
				if(currentY < 18)
					currentY++;
				break;
			}

			break;
		}

		if(nearbyObject < 7)
		{
			if(biomes[currentX] == FLAT)
			{
				if(currentX > 15 && currentX < X_BLOCKS - 15 && elevations[currentX] > 5 && nextInt(8) == 0 && nearbyMonster == 0)
				{
					addEntity(new EntitySlime(currentX * BLOCKSIZE, (currentY * BLOCKSIZE) - EntitySlime::getStartingHeight(), this));
					nearbyMonster = 7;
				}
			}

			if(currentX > 15 && currentX < X_BLOCKS - 15 && elevations[currentX] > 5 && nextInt(2) == 0 && nearbyMonster == 0)
			{
				addEntity(new EntityWoompa(currentX * BLOCKSIZE, (currentY * BLOCKSIZE) - EntityWoompa::getStartingHeight(), this));
				nearbyMonster = 6;
			}
		}
	}

	for(int i = 0; i < currentY; i++)
	{
		setBlock(X_BLOCKS - 1 , i, portal);
	}
}