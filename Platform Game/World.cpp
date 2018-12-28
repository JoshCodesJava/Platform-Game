#include <iostream>
#include <fstream>
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

World::World(AudioLoader* loader) : paused(false), xscroll(0), abortUpdate(false)
{
	this->loader = loader;
	ground = new BlockGround();
	sponge = new BlockSponge();
	lava = new BlockLava();
	ladder = new BlockLadder();
	portal = new BlockPortal();
	diamond = new BlockDiamond();
	checkpoint = new BlockCheckpoint();
	dispenser = new BlockDispenser();
	character = new EntityCharacter(this);
	addEntity(character);
	loadLevel();
}

void World::keyPressed(int key) 
{
	for(unsigned int i = 0; i < entities.size(); i++)
	{
		entities[i]->keyPressed(key);
	}
}

void World::keyReleased(int key) 
{
	for(unsigned int i = 0; i < entities.size(); i++)
	{
		entities[i]->keyReleased(key);
	}
}

bool World::run()
{
	if(!paused)
	{
		updateTiles();
		checkCollisions();

		for(int i = entities.size() - 1; i >= 0; i--)
		{
			Entity* entity = entities[i];

			if(entity->toBeDeleted())
			{
				removeEntity(entity);
				delete entity;
			}
		}

		abortUpdate = false;
	}

	return terminated;
}

void World::updateTiles()
{
	for(unsigned int i = 0; i < entities.size() && !abortUpdate; i++)
	{
		Entity* entity = entities[i];

		if(!entity->toBeDeleted())
		{
			if((float)entity->getX()*SCALE>=(float)xscroll*SCALE-BLOCKSIZE*SCALE && (float)entity->getX()*SCALE<=(float)xscroll*SCALE+WIDTH)
			{
				entity->update(true);
			}
			else
				entity->update(false);
		}
	}

	for(int x = (int)floor((float) getXs()*SCALE / (BLOCKSIZE*SCALE)); x<((int) ceil((float) (getXs()*SCALE + WIDTH) / (BLOCKSIZE*SCALE))); x++)
	{
		for(int y = 0; y<Y_BLOCKS; y++)
		{
			if(!abortUpdate)
			{
				Block *block = getBlock(x, y);

				if(block != 0)
				{
					block->update(x, y);
				}
			}
		}
	}
}

void World::checkCollisions()
{
	ContainRectangle rect_block = ContainRectangle(0,0,0,0);
	ContainRectangle rect_up = ContainRectangle(0,0,0,0);
	ContainRectangle rect_down = ContainRectangle(0,0,0,0);
	ContainRectangle rect_left = ContainRectangle(0,0,0,0);
	ContainRectangle rect_right = ContainRectangle(0,0,0,0);
	ContainRectangle rect_entity = ContainRectangle(0,0,0,0);
	ContainRectangle rect_other = ContainRectangle(0,0,0,0);

	for(unsigned int i = 0; i < entities.size() && !abortUpdate; i++)
	{
		Entity *ent = entities[i];

		if(!ent->toBeDeleted())
		{
			if((float) ent->getX()*SCALE>=(float) getXs() * SCALE - BLOCKSIZE && (float) ent->getX() * SCALE <= (float) getXs()*SCALE+WIDTH)
			{
				for(int x = (int)floor((float) getXs()*SCALE / (BLOCKSIZE*SCALE)); x<((int) ceil((float) (getXs()*SCALE + WIDTH) / (BLOCKSIZE*SCALE))); x++)
				{
					for(int y = 0; y<Y_BLOCKS; y++)
					{
						if(!ent->toBeDeleted() && !abortUpdate)
						{
							Block* check = getBlock(x,y);

							if(check!=NULL)
							{
								rect_entity.setFrame(ent->getX(), ent->getY(), ent->getWidth(), ent->getHeight());
								int blockX = x*BLOCKSIZE;
								int blockY = y*BLOCKSIZE;
								rect_block.setFrame(blockX, blockY, BLOCKSIZE, BLOCKSIZE);

								if(rect_block.intersects(rect_entity))
								{
									check->intersectsWithEntity(ent, x, y);
								}
								else
								{
									rect_up.setFrame(ent->getX(), ent->getY()-1, ent->getWidth(), ent->getHeight());
									rect_down.setFrame(ent->getX(), ent->getY()+1, ent->getWidth(), ent->getHeight());
									rect_left.setFrame(ent->getX()-1, ent->getY(), ent->getWidth(), ent->getHeight());
									rect_right.setFrame(ent->getX()+1, ent->getY(), ent->getWidth(), ent->getHeight());

									if(rect_block.intersects(rect_up))
									{
										check->bordersEntity(UP, ent, blockX, blockY);
									}

									else if(rect_block.intersects(rect_down))
									{
										check->bordersEntity(DOWN, ent, blockX, blockY);
									}

									else if(rect_block.intersects(rect_left))
									{
										check->bordersEntity(LEFT, ent, blockX, blockY);
									}

									else if(rect_block.intersects(rect_right))
									{
										check->bordersEntity(RIGHT, ent, blockX, blockY);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	for(unsigned int i = 0; i < entities.size() && !abortUpdate; i++)
	{
		Entity *ent = entities[i];
		rect_entity.setFrame(ent->getX(), ent->getY(), ent->getWidth(), ent->getHeight());

		if(!ent->toBeDeleted())
		{
			for(unsigned int i = 0; i < entities.size() && !abortUpdate; i++)
			{
				Entity *check = entities[i];

				if(!ent->toBeDeleted() && !check->toBeDeleted())
				{
					rect_other.setFrame(check->getX(), check->getY(), check->getWidth(), check->getHeight());

					if(check!=ent)
					{
						rect_up.setFrame(rect_entity.getX(), rect_entity.getY()-2, rect_entity.getWidth(), rect_entity.getHeight());
						rect_down.setFrame(rect_entity.getX(), rect_entity.getY()+2, rect_entity.getWidth(), rect_entity.getHeight());
						rect_left.setFrame(rect_entity.getX()-2, rect_entity.getY(), rect_entity.getWidth(), rect_entity.getHeight());
						rect_right.setFrame(rect_entity.getX()+2, rect_entity.getY(), rect_entity.getWidth(), rect_entity.getHeight());

						if(rect_other.intersects(rect_up))
							ent->bordersEntity(UP, check);

						else if(rect_other.intersects(rect_down))
							ent->bordersEntity(DOWN, check);

						else if(rect_other.intersects(rect_left))
							ent->bordersEntity(LEFT, check);

						else if(rect_other.intersects(rect_right))
							ent->bordersEntity(RIGHT, check);
					}
				}
			}
		}
	}
}

void World::pause() 
{
	paused = true;
}

void World::resume()
{
	paused = false;
}

void World::clear()
{
	entities.clear();
}

int World::getXs()
{
	return xscroll;
}

void World::setXs(int xs)
{
	xscroll = xs;
}

EntityCharacter* World::getCharacter()
{
	return character;
}

World::~World()
{
	delete ground;
	delete sponge;
	delete lava;
	delete ladder;
	delete portal;
	delete diamond;
	delete checkpoint;
	delete dispenser;

	for(unsigned int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}
}