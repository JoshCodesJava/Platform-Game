#include "BlockCheckpoint.h"
#include "EntityCharacter.h"

void BlockCheckpoint::intersectsWithEntity(Entity* ent, int x, int y) 
{
	if(typeid(*ent) == typeid(EntityCharacter))
	{
		if(!(theWorld->getMetaData(x,y) == "activated"))
		{
			theWorld->setMetaData("activated", x, y);
			((EntityCharacter*) ent)->save();
			theWorld->playSound("sound/checkpoint.wav");
		}
	}
}