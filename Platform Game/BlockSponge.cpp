#include "BlockSponge.h"
#include "EntityCharacter.h"

void BlockSponge::bordersEntity(Direction dir, Entity *entity, int x, int y)
{
	if(dir == DOWN && typeid(*entity) == typeid (EntityCharacter))
	{
		((EntityCharacter*) entity)->jump();
		theWorld->playSound("sound/bounce.wav");
	}
}