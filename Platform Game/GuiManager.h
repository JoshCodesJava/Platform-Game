#pragma once
#include <TGUI/TGUI.hpp>
#include <string>
#include <iostream>
#include <map>
#include "Constants.h"
#define NEWGAME_ID 0
#define OPTIONS_ID 1
#define EXIT_ID 2
#define CREDITS_ID 3
#define MENU_ID 4
#define SOUND_ID 5
#define MUSIC_ID 6
#define LEVELMODE_ID 7
#define BACKTOGAME_ID 8

using namespace std;

class GUIManager
{
private:
	string dir;
	int soundVolume;
	int musicVolume;
	map<string, int> options;

public:
	GUIManager()
	{
		ifstream file("settings.conf");
		string line;
		setDefaults();

		if(file.is_open())
		{
			while (getline(file, line))
			{
				stringstream temp;
				temp<<line;

				string key;
				temp>>key;
				int value;
				temp>>value;
				options[key] = value;
			}
		}

		file.close();
	}

	void setDefaults()
	{
		options["music-volume"] = 50;
		options["sound-volume"] = 50;
	}

	tgui::Panel::Ptr getStartPanel()
	{
		static bool init = false;
		static tgui::Panel::Ptr panel;

		if(!init)
		{
			panel->setGlobalFont("gui/Stanislav.ttf");
			panel->setSize((float) WIDTH, (float) HEIGHT);
			tgui::Picture::Ptr picture(*panel);
			picture->load("gui/Background.png");
			picture->setSize((float) WIDTH, (float) HEIGHT);
			tgui::Grid::Ptr grid(*panel);
			grid->setSize(600, 225);
			grid->setPosition((float)(WIDTH/2) - (600/2), (float)(HEIGHT/2) - (140/2));
			tgui::Button::Ptr levelMode(*grid);
			levelMode->load("gui/Black.conf");
			levelMode->setSize(600, 40);
			levelMode->setText("Level Mode");
			levelMode->setCallbackId(LEVELMODE_ID);
			levelMode->bindCallback(tgui::Button::LeftMouseClicked);
			grid->addWidget(levelMode, 0, 0);
			tgui::Button::Ptr newGame(*grid);
			newGame->load("gui/Black.conf");
			newGame->setSize(600, 40);
			newGame->setText("Random Level");
			newGame->setCallbackId(NEWGAME_ID);
			newGame->bindCallback(tgui::Button::LeftMouseClicked);
			grid->addWidget(newGame, 1, 0);
			tgui::Button::Ptr options(*grid);
			options->load("gui/Black.conf");
			options->setSize(600, 40);
			options->setText("Options");
			options->setCallbackId(OPTIONS_ID);
			options->bindCallback(tgui::Button::LeftMouseClicked);
			grid->addWidget(options, 2, 0);
			tgui::Button::Ptr credits(*grid);
			credits->load("gui/Black.conf");
			credits->setSize(600, 40);
			credits->setText("Credits");
			credits->setCallbackId(CREDITS_ID);
			credits->bindCallback(tgui::Button::LeftMouseClicked);
			grid->addWidget(credits, 3, 0);
			tgui::Button::Ptr exit(*grid);
			exit->load("gui/Black.conf");
			exit->setSize(600, 40);
			exit->setText("Exit");
			exit->setCallbackId(EXIT_ID);
			exit->bindCallback(tgui::Button::LeftMouseClicked);
			grid->addWidget(exit, 4, 0);
			init = true;
		}

		return panel;
	}

	tgui::Panel::Ptr getCreditPanel()
	{
		static bool init = false;
		static tgui::Panel::Ptr panel;

		if(!init)
		{
			panel->setGlobalFont("gui/Stanislav.ttf");
			panel->setSize((float) WIDTH, (float) HEIGHT);
			tgui::Picture::Ptr picture(*panel);
			picture->load("gui/Background.png");
			picture->setSize((float) WIDTH, (float) HEIGHT);
			tgui::Grid::Ptr grid(*panel);
			grid->setSize(600, 300);
			grid->setPosition((float)(WIDTH/2) - (600/2), (float)(HEIGHT/2) - (300/2));
			tgui::ListBox::Ptr creditBox(*grid);
			creditBox->load("gui/Black.conf");
			creditBox->setSize(600, 250);
			creditBox->addItem("Programmer - Josh Sobel");
			creditBox->addItem("Artists - Josh Sobel & Owen Kirk-Whitman");
			creditBox->addItem("Music - Josh Sobel & Owen Kirk-Whitman");
			creditBox->addItem("");
			creditBox->addItem("Used Libraries");
			creditBox->addItem("	- SFML");
			creditBox->addItem("	- TGUI");
			grid->addWidget(creditBox, 0, 0);
			tgui::Button::Ptr menu(*grid);
			menu->load("gui/Black.conf");
			menu->setSize(600, 40);
			menu->setText("Main Menu");
			menu->setCallbackId(MENU_ID);
			menu->bindCallback(tgui::Button::LeftMouseClicked);
			grid->addWidget(menu, 1, 0);
			init = true;
		}

		return panel;
	}

	tgui::Panel::Ptr getOptionPanel()
	{
		static bool init = false;
		static tgui::Panel::Ptr panel;

		if(!init)
		{
			panel->setGlobalFont("gui/Stanislav.ttf");
			panel->setSize((float) WIDTH, (float) HEIGHT);
			tgui::Picture::Ptr picture(*panel);
			picture->load("gui/Background.png");
			picture->setSize((float) WIDTH, (float) HEIGHT);
			tgui::Grid::Ptr grid(*panel);
			grid->setSize(600, 300);
			grid->setPosition((float)(WIDTH/2) - (600/2), (float)(HEIGHT/2) - (300/2));
			tgui::Label::Ptr music(*grid);
			music->setText("Music Volume");
			music->setSize(600, 40);
			music->setTextColor(sf::Color());
			grid->addWidget(music, 0, 0);
			tgui::Slider::Ptr musicVolume(*grid);
			musicVolume->load("gui/Black.conf");
			musicVolume->setSize(600, 40);
			musicVolume->setVerticalScroll(false);
			musicVolume->setMinimum(0);
			musicVolume->setMaximum(100);
			musicVolume->setValue(options["music-volume"]);
			musicVolume->setCallbackId(MUSIC_ID);
			musicVolume->bindCallback(tgui::Slider::ValueChanged);
			grid->addWidget(musicVolume, 1, 0);
			tgui::Label::Ptr sound(*grid);
			sound->setText("Sound Volume");
			sound->setSize(600, 40);
			sound->setTextColor(sf::Color());
			grid->addWidget(sound, 2, 0);
			tgui::Slider::Ptr soundVolume(*grid);
			soundVolume->load("gui/Black.conf");
			soundVolume->setSize(600, 40);
			soundVolume->setVerticalScroll(false);
			soundVolume->setMinimum(0);
			soundVolume->setMaximum(100);
			soundVolume->setValue(options["sound-volume"]);
			soundVolume->setCallbackId(SOUND_ID);
			soundVolume->bindCallback(tgui::Slider::ValueChanged);
			grid->addWidget(soundVolume, 3, 0);
			tgui::Button::Ptr menu(*grid);
			menu->load("gui/Black.conf");
			menu->setSize(600, 40);
			menu->setText("Main Menu");
			menu->setCallbackId(MENU_ID);
			menu->bindCallback(tgui::Button::LeftMouseClicked);
			grid->addWidget(menu, 4, 0);
			init = true;
		}

		return panel;
	}

	tgui::Panel::Ptr getOptionPanelInGame()
	{
		static bool init = false;
		static tgui::Panel::Ptr panel;

		if(!init)
		{
			panel->setGlobalFont("gui/Stanislav.ttf");
			panel->setSize((float) WIDTH, (float) HEIGHT);
			tgui::Picture::Ptr picture(*panel);
			picture->load("gui/Background.png");
			picture->setSize((float) WIDTH, (float) HEIGHT);
			tgui::Grid::Ptr grid(*panel);
			grid->setSize(600, 340);
			grid->setPosition((float)(WIDTH/2) - (600/2), (float)(HEIGHT/2) - (340/2));
			tgui::Label::Ptr music(*grid);
			music->setText("Music Volume");
			music->setSize(600, 40);
			music->setTextColor(sf::Color());
			grid->addWidget(music, 0, 0);
			tgui::Slider::Ptr musicVolume(*grid);
			musicVolume->load("gui/Black.conf");
			musicVolume->setSize(600, 40);
			musicVolume->setVerticalScroll(false);
			musicVolume->setMinimum(0);
			musicVolume->setMaximum(100);
			musicVolume->setValue(options["music-volume"]);
			musicVolume->setCallbackId(MUSIC_ID);
			musicVolume->bindCallback(tgui::Slider::ValueChanged);
			grid->addWidget(musicVolume, 1, 0);
			tgui::Label::Ptr sound(*grid);
			sound->setText("Sound Volume");
			sound->setSize(600, 40);
			sound->setTextColor(sf::Color());
			grid->addWidget(sound, 2, 0);
			tgui::Slider::Ptr soundVolume(*grid);
			soundVolume->load("gui/Black.conf");
			soundVolume->setSize(600, 40);
			soundVolume->setVerticalScroll(false);
			soundVolume->setMinimum(0);
			soundVolume->setMaximum(100);
			soundVolume->setValue(options["sound-volume"]);
			soundVolume->setCallbackId(SOUND_ID);
			soundVolume->bindCallback(tgui::Slider::ValueChanged);
			grid->addWidget(soundVolume, 3, 0);
			tgui::Button::Ptr exit(*grid);
			exit->load("gui/Black.conf");
			exit->setSize(600, 40);
			exit->setText("Return To Game");
			exit->setCallbackId(BACKTOGAME_ID);
			exit->bindCallback(tgui::Button::LeftMouseClicked);
			grid->addWidget(exit, 4, 0);

			tgui::Button::Ptr menu(*grid);
			menu->load("gui/Black.conf");
			menu->setSize(600, 40);
			menu->setText("Exit To Menu");
			menu->setCallbackId(MENU_ID);
			menu->bindCallback(tgui::Button::LeftMouseClicked);
			grid->addWidget(menu, 5, 0);
			init = true;
		}

		return panel;
	}

	float getOption(string setting)
	{
		return options[setting];
	}

	void guiEvent(int id, int value)
	{
		if(id == MUSIC_ID)
		{
			options["music-volume"] = value;
		}
		else if(id == SOUND_ID)
		{
			options["sound-volume"] = value;
		}
	}

	void saveState()
	{
		ofstream file("settings.conf");
		file<<"music-volume "<<options["music-volume"]<<endl;
		file<<"sound-volume "<<options["sound-volume"];
		file.flush();
		file.close();
	}
};