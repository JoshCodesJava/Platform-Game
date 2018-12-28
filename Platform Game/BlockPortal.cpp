#include "BlockPortal.h"
#include "EntityCharacter.h"

void BlockPortal::intersectsWithEntity(Entity *entity, int x, int y) 
{
	if(typeid(*entity) == typeid (EntityCharacter))
	{
		((EntityCharacter*) entity)->win();
	}
}