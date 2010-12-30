#ifndef MODE_H
#define MODE_H

#include "game.h"
#include "timer.h"

const std::string SPRINT_BG_IMAGE = "/Users/Feysal/Flocks/images/sprint_bg.png";

class Sprint : public Game
{
private:
	Timer timer;
	bool gameOver;
	
public:
	Sprint(sf::RenderWindow *renderArea) : Game(renderArea) { gameOver = false; timer.reset(); }
	
	void handleGameOverInput();
	
	void render();
	void play();
};

#endif
