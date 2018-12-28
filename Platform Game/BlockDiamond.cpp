#include "BlockDiamond.h"
#include "EntityCharacter.h"

void BlockDiamond::intersectsWithEntity(Entity *entity, int x, int y) 
{
	if(typeid(*entity) == typeid (EntityCharacter))
	{
		((EntityCharacter*) entity)->addToScore(1);
		theWorld->setBlock(x, y, NULL);
		theWorld->playSound("sound/diamond.wav");
	}
}