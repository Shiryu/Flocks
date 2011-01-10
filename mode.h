#ifndef MODE_H
#define MODE_H

#include "game.h"
#include "timer.h"

const std::string ULTRA_BG_IMG = "images/ultra.png";
const std::string SPRINT_BG_IMG = "images/sprint.png";
const std::string TIME_ELAPSED_IMG = "images/timeelapsed.png";
const std::string SPRINT_FINISHED_IMG = "images/sprintfinished.png";
const std::string SPRINT_GAMEOVER_IMG = "images/sprintgameover.png";

const std::string DS_DIGITAL_FONT = "fonts/ds_digital/DS-DIGI.ttf";

const std::string SPRINT_DATA_FILE = "data/sprint.flk";
const std::string ULTRA_DATA_FILE = "data/ultra.flk";

const int NB_LINES_TO_DELETE = 40;

class Ultra : public Game
{
private:
	Timer timer;
	
public:
	Ultra(sf::RenderWindow *renderArea) : Game(renderArea) { timer.reset(); }
	~Ultra();
	
	void restart();
	
	void showInfos();
	
	void recordBestScore();
	void showBestScore();
	
	bool timeElapsed();
	
	int handlePauseInput();
	int handleGameOverInput();
	
	void render();
	void play();
};

class Sprint : public Game
{
private:
	Timer timer;
	int nbLinesToDelete;
	
public:
	Sprint(sf::RenderWindow *renderArea) : Game(renderArea) { timer.reset(); nbLinesToDelete = NB_LINES_TO_DELETE; }
	~Sprint();
	
	void setNbLinesToDelete(int n) { nbLinesToDelete = n; }
	int getNbLinesToDelete() { return nbLinesToDelete; }
	
	void restart();
	
	void showInfos();
	
	void recordBestScore();
	void showBestScore();
	
	bool finished();
	
	int handlePauseInput();
	int handleGameOverInput();
	
	void render();
	void play();
};

#endif
