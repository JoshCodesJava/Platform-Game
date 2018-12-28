#include "BlockLava.h"
#include "EntityCharacter.h"

void BlockLava::intersectsWithEntity(Entity *entity, int x, int y) 
{
	if(typeid(*entity) == typeid (EntityCharacter))
	{
		((EntityCharacter*) entity)->die();
	}
	else
	{
		entity->setForDeletion();
	}
}