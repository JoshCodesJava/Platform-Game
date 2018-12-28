#include "EntityFireball.h"
#include "EntityCharacter.h"
#include "EntityDiamond.h"

void EntityFireball::bordersEntity(Direction dir, Entity* check) 
{
	if(!((typeid(*check) == typeid(EntityDiamond) || (typeid(*check) == typeid(EntityFireball)))))
	{
		setForDeletion();

		if(check == theWorld->getCharacter())
			((EntityCharacter*) check)->die();
		else
			check->setForDeletion();
	}
}