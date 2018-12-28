#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Utility.h"
#include <vector>

using namespace std;

class EntityCharacter;
class World;
class Entity;
class Block;

enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Entity
{
private:
	int x;
	int y;
	bool deleted;

protected:
	World* theWorld;

public:
	Entity(int x, int y, World* world)
	{
		this->x = x;
		this->y = y;
		this->theWorld = world;
		deleted = false;
	}

	void setForDeletion()
	{
		deleted = true;
	}

	bool toBeDeleted()
	{
		return deleted;
	}

	virtual void update(bool inRange) = 0;

	virtual string getImage() = 0;

	virtual bool impermanent()
	{
		return false;
	}

	virtual int getX() 
	{
		return x;
	}

	virtual void setX(int x)
	{
		this->x = x;
	}

	virtual int getY()
	{
		return y;
	}

	virtual void setY(int y) 
	{
		this->y = y;
	}

	virtual int getTextureWidth() = 0;
	
	virtual int getTextureHeight() = 0;

	virtual int getWidth() = 0;

	virtual int getHeight() = 0;

	virtual void keyPressed(int key)
	{
	}

	virtual void keyReleased(int key)
	{
	}

	virtual void bordersEntity(Direction dir, Entity *check) 
	{
	}

	virtual Entity* clone() = 0;

	virtual ~Entity()
	{

	}
};

class Block
{
public:
	static World* theWorld;

	virtual string getImage(int x, int y) = 0;

	virtual void update(int x, int y)
	{

	}

	virtual bool isSolid()
	{
		return true;
	} 

	virtual void bordersEntity(Direction dir, Entity *entity, int x, int y)
	{
	}

	virtual void intersectsWithEntity(Entity *entity, int x, int y) 
	{
	}

	virtual ~Block()
	{

	}
};

struct MAP
{
	Block* blocks[X_BLOCKS][Y_BLOCKS];
	string metadata[X_BLOCKS][Y_BLOCKS];
};

class World
{
protected:
	EntityCharacter* character;
	vector<Entity*> entities;
	MAP map;
	bool paused;
	int xscroll;
	bool abortUpdate;
	bool terminated;
	AudioLoader* loader;

	void checkCollisions();

	void updateTiles();

public:
	Block* ground;
	Block* sponge;
	Block* lava;
	Block* ladder;
	Block* portal;
	Block* diamond;
	Block* checkpoint;
	Block* dispenser;

	World(AudioLoader* loader);

	void playSound(string sound)
	{
		loader->playSound(sound);
	}

	void terminate()
	{
		terminated = true;
	}

	void abort()
	{
		abortUpdate = true;
	}

	void keyPressed(int key);

	void keyReleased(int key);

	bool run();

	void pause();

	void resume();

	vector<Entity*> getEntities()
	{
		return entities;
	}

	MAP getMap()
	{
		return map;
	}

	void clear();

	void addEntity(Entity *e)
	{
		entities.push_back(e);
	}

	void removeEntity(Entity *e)
	{
		for(unsigned int i = 0; i < entities.size(); i++)
		{
			if(entities[i] == e)
			{
				entities.erase(entities.begin()+i);
				break;
			}
		}
	}

	void setBlock(int x, int y, Block *block)
	{
		map.blocks[x][y] = block;
	}

	Block* getBlock(int x, int y)
	{
		if(x < 0 || y < 0 || x >= X_BLOCKS || y >= Y_BLOCKS)
		{
			return 0;
		}

		return map.blocks[x][y];
	}

	int getXs();

	void setXs(int xs);

	string getMetaData(int x, int y)
	{
		if(x < 0 || y < 0 || x >= X_BLOCKS || y >= Y_BLOCKS)
		{
			return 0;
		}

		return map.metadata[x][y];
	}

	void setMetaData(string data, int i, int j)
	{
		map.metadata[i][j] = data;
	}

	EntityCharacter* getCharacter();

	~World();

private:
	void loadLevel();
};