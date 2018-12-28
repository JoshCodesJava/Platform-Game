#pragma once
#include <map>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#define M_PI 3.141592653589793
#define toRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define toDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

using namespace std;

class ImageLoader
{
private:
	map<string, sf::Texture*> keys;
	vector<sf::Texture*> textures;

public:
	sf::Texture* getImage(string key)
	{
		if(keys[key] != NULL)
			return keys[key];
		else
		{
			sf::Texture* texture = new sf::Texture();
			texture->loadFromFile(key);
			textures.push_back(texture);
			keys[key] = texture;
			return keys[key];
		}
	}

	~ImageLoader()
	{
		for(unsigned int i = 0; i < textures.size(); i++)
			delete textures[i];
	}
};

class AudioLoader
{
private:
	map<string, sf::Sound*> map;
	vector<sf::SoundBuffer*> buffers;
	vector<sf::Sound*> sounds;
	int volume;

public:
	AudioLoader()
	{
		volume = 50;
	}

	void setSoundVolume(int value)
	{
		volume = value;
	}

	void playSound(string sound)
	{
		if(map[sound] == NULL)
		{
			sf::SoundBuffer* buffer = new sf::SoundBuffer();
			buffer->loadFromFile(sound);
			sf::Sound* playsound = new sf::Sound(*buffer);
			buffers.push_back(buffer);
			sounds.push_back(playsound);
			map[sound] = playsound;
		}

		map[sound]->setVolume(volume);
		map[sound]->play();
	}

	~AudioLoader()
	{
		for(unsigned int i = 0; i < sounds.size(); i++)
		{
			delete sounds[i];
			delete buffers[i];
		}
	}
};

class Point
{
private:
	int x,y;

public:
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int getX()
	{
		return x;
	}

	int getY()
	{
		return y;
	}

	void setLocation(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};
class ContainRectangle
{
protected:
	int x, y, width, height;

public:
	ContainRectangle(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	void setFrame(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	int getX()
	{
		return x;
	}

	int getY()
	{
		return y;
	}

	int getWidth()
	{
		return width;
	}

	int getHeight()
	{
		return height;
	}

	bool intersects(ContainRectangle rect)
	{
		int x = rect.getX();
		int y = rect.getY();
		int w = rect.getWidth();
		int h = rect.getHeight();

		if (w <= 0 || h <= 0) 
		{
			return false;
		}

		int x0 = getX();
		int y0 = getY();
		return (x + w > x0 && y + h > y0 && x < x0 + getWidth() && y < y0 + getHeight());
	}

	bool contains(Point p)
	{
		return contains(p.getX(), p.getY());
	}

	bool contains(int x, int y)
	{
		int x0 = getX();
		int y0 = getY();
		return (x >= x0 && y >= y0 && x < x0 + getWidth() && y < y0 + getHeight());
	}
};