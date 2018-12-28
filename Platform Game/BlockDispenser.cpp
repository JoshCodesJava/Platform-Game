#include "BlockDispenser.h"
#include "EntityCharacter.h"
#include "EntityFireball.h"

double BlockDispenser::calcAngle(Point p1, Point p2)
{
	double xDiff = p2.getX() - p1.getX();
	double yDiff = p2.getY() - p1.getY();
	return toDegrees(atan2(yDiff, xDiff));
}

void BlockDispenser::update(int x, int y)
{
	if(atoi(theWorld->getMetaData(x, y).c_str()) > 1000)
	{ 
		int playerX = (theWorld->getCharacter()->getX()+theWorld->getCharacter()->getX()+theWorld->getCharacter()->getWidth())/2;
		int playerY = (theWorld->getCharacter()->getY()+theWorld->getCharacter()->getY()+theWorld->getCharacter()->getHeight())/2;
		int dispenserX = x*BLOCKSIZE;
		int dispenserY = y*BLOCKSIZE;
		Point player(playerX, playerY);
		Point dispenser(dispenserX, dispenserY);
		double angle = calcAngle(dispenser, player);
		theWorld->addEntity(new EntityFireball(x*BLOCKSIZE,y*BLOCKSIZE,angle,1,theWorld));
		theWorld->playSound("sound/dispenser.wav");
		theWorld->setMetaData("0", x, y);
	}
	else
	{
		int time = atoi(theWorld->getMetaData(x, y).c_str());
		time++;
		char buff[5];
		_itoa_s(time, buff, 5, 10);
		theWorld->setMetaData(buff, x, y);
	}
}