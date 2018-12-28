#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include "Constants.h"
#include "GuiManager.h"
#include "Environment.h"
#include "EntityCharacter.h"

using namespace std;

World* Block::theWorld;

const int BACKGROUNDIMAGEWIDTH = 1366;
const int BACKGROUNDIMAGEHEIGHT = 768;
const float BACKGROUNDIMAGESCALEX = (float) WIDTH/BACKGROUNDIMAGEWIDTH;
const float BACKGROUNDIMAGESCALEY = (float) HEIGHT/BACKGROUNDIMAGEHEIGHT;
World* world;
ImageLoader* loader;
AudioLoader* aloader;
sf::Font font;
sf::Text text;
bool showGui = true;
bool gameInProgress = false;
bool fullscreen = true;

bool update();
bool sync();
void draw(sf::RenderWindow &window);
void keyPressed(int keycode);
void keyReleased(int keycode);

int main(int length, char* args[])
{
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Platform Game", sf::Style::Fullscreen);
	GUIManager manager;

	try
	{
		loader = new ImageLoader();
		aloader = new AudioLoader();
		sf::Music loop;
		loop.openFromFile("sound/loop.wav");
		loop.setLoop(true);
		loop.setVolume(manager.getOption("music-volume"));
		aloader->setSoundVolume(manager.getOption("sound-volume"));
		font.loadFromFile("resources/PressStart2P.ttf");
		text.setFont(font);
		text.setCharacterSize(25);
		text.setStyle(sf::Text::Bold);
		text.setColor(sf::Color::Red);
		tgui::Gui gui(window);
		gui.add(manager.getStartPanel());
		sf::Event event;

		while(window.isOpen())
		{ 
			while (window.pollEvent(event))
			{
				if(showGui)
					gui.handleEvent(event);

				if(event.type == sf::Event::KeyPressed)
				{
					if(event.key.code == sf::Keyboard::Escape && !showGui)
					{
						loop.stop();
						gui.removeAllWidgets();
						gui.add(manager.getOptionPanelInGame());	
						showGui = true;
						gameInProgress = true;
						window.setMouseCursorVisible(true);
					}
					else if(event.key.code == sf::Keyboard::F11)
					{
						fullscreen = !fullscreen;

						if(fullscreen)
							window.create(sf::VideoMode(WIDTH, HEIGHT), "Platform Game", sf::Style::Fullscreen);
						else
							window.create(sf::VideoMode(WIDTH, HEIGHT), "Platform Game", sf::Style::Default);
					}
					else
					{
						if(!showGui)
							keyPressed(event.key.code);
					}
				}
				else if(event.type == sf::Event::KeyReleased)
				{
					if(!showGui)
						keyReleased(event.key.code);
				}
				else if(event.type == sf::Event::LostFocus)
				{
					fullscreen = false;
					window.create(sf::VideoMode(WIDTH, HEIGHT), "Platform Game", sf::Style::Default);
				}
				else if(event.type == sf::Event::Closed)
				{
					window.close();
				}
			}

			if(showGui)
			{
				tgui::Callback callback;

				while (gui.pollCallback(callback))
				{
					manager.guiEvent(callback.id, callback.value);

					if (callback.id == NEWGAME_ID)
					{
						loop.play();
						world = new World(aloader);
						Block::theWorld = world;
						window.setMouseCursorVisible(false);
						showGui = false;
					}
					else if (callback.id == BACKTOGAME_ID)
					{
						loop.play();
						window.setMouseCursorVisible(false);
						showGui = false;
					}
					else if (callback.id == EXIT_ID)
					{
						return 0;
					}
					else if (callback.id == CREDITS_ID)
					{
						gui.removeAllWidgets();
						gui.add(manager.getCreditPanel());
					}
					else if (callback.id == MENU_ID)
					{
						if(gameInProgress)
						{
							gameInProgress = false;
							delete world;
						}

						gui.removeAllWidgets();
						gui.add(manager.getStartPanel());		
						manager.saveState();
					}
					else if (callback.id == OPTIONS_ID)
					{
						gui.removeAllWidgets();
						gui.add(manager.getOptionPanel());				
					}
					else if (callback.id == MUSIC_ID)
					{
						loop.setVolume(callback.value);
					}
					else if (callback.id == SOUND_ID)
					{
						aloader->setSoundVolume(callback.value);			
					}
				}
			}

			if(!showGui)
			{
				if(sync())
				{
					loop.stop();
					delete world;
					window.setMouseCursorVisible(true);
					showGui = true;
				}
				else
				{
					window.clear();
					draw(window);
					window.display();
				}
			}
			else
			{
				window.clear();
				gui.draw();
				window.display();
			}
		}
	}
	catch(exception e)
	{
		window.close();
		MessageBox(NULL, L"ERROR", L"ERROR", MB_OK);
	}

	delete loader;
	delete aloader;

}

bool sync()
{
	static sf::Clock clock;
	static int totalTime = 0;
	int pastMillis = clock.getElapsedTime().asMilliseconds();
	clock.restart();
	totalTime += pastMillis;

	if(totalTime > (10/9))
	{
		totalTime = 0;
		return update();
	}

	return false;
}

bool update()
{
	return world->run();
}

string getCompletePercent()
{
	float percent = ((float) world->getCharacter()->getX() / ((float) X_BLOCKS * (float) BLOCKSIZE)) * 100;
	stringstream ss;
	ss<<percent;
	string s = ss.str();
	return s.substr(0, s.find_first_of('.') + 2);
}

void draw(sf::RenderWindow &window)
{
	for(int x = (int)floor((float) world->getXs()*SCALE / (BACKGROUNDIMAGEWIDTH * BACKGROUNDIMAGESCALEX)); x<((int) ceil((float) (world->getXs()*SCALE + WIDTH) / (BACKGROUNDIMAGEWIDTH * BACKGROUNDIMAGESCALEX))); x++)
	{
		sf::Sprite sprite(*loader->getImage("images/Background.png"));
		sprite.scale(BACKGROUNDIMAGESCALEX, BACKGROUNDIMAGESCALEY);
		sprite.setPosition((float)(x * BACKGROUNDIMAGEWIDTH * BACKGROUNDIMAGESCALEX) - world->getXs() * SCALE, 0);
		window.draw(sprite);
	}

	for(int x = (int)floor((float) world->getXs()*SCALE / (BLOCKSIZE*SCALE)); x<((int) ceil((float) (world->getXs()*SCALE + WIDTH) / (BLOCKSIZE*SCALE))); x++)
	{
		for(int y = 0; y<Y_BLOCKS; y++)
		{
			Block* block = world->getBlock(x, y);

			if(block != NULL)
			{
				sf::Sprite blockImage(*(loader->getImage(block->getImage(x, y))));
				blockImage.setScale(SCALE, SCALE);
				blockImage.move((float) (x*BLOCKSIZE*SCALE) - (world->getXs() * SCALE), (float) (y*BLOCKSIZE*SCALE) + (HEIGHT - (BLOCKSIZE * Y_BLOCKS * SCALE)));
				window.draw(blockImage);
			}
		}
	}

	for(unsigned int i = 0; i < world->getEntities().size(); i++)
	{
		Entity* entity = world->getEntities()[i];

		if(entity->getX()*SCALE>=world->getXs()*SCALE-BLOCKSIZE*SCALE && entity->getX()*SCALE<=world->getXs()*SCALE+WIDTH)
		{
			sf::Sprite entitySprite(*(loader->getImage(entity->getImage())));
			float SCALE_X = (float)(entity->getWidth() * SCALE) / (float)(entity->getTextureWidth());
			float SCALE_Y = (float)(entity->getHeight() * SCALE) / (float)(entity->getTextureHeight());
			entitySprite.setScale(SCALE_X, SCALE_Y);
			entitySprite.move((float) (entity->getX() * SCALE) - (world->getXs() * SCALE), (float) (entity->getY() * SCALE) + (HEIGHT - (BLOCKSIZE * Y_BLOCKS * SCALE)));
			window.draw(entitySprite);
		}
	}

	stringstream ss;
	ss << "Lives " << world->getCharacter()->getRetries() + 1;
	ss << "   Stars " << world->getCharacter()->getScore();
	ss << "   Percent Complete "+getCompletePercent()+"%";
	string textString = ss.str();
	float stringX = (float)(WIDTH/2) - ((textString.length() * 25)/2);
	text.setPosition(stringX, 27);
	text.setString(textString);
	window.draw(text);
}

void keyPressed(int keycode)
{
	world->keyPressed(keycode);
}

void keyReleased(int keycode)
{
	world->keyReleased(keycode);
}