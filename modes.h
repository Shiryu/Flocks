#ifndef MODE_H
#define MODE_H

#include "game.h"
#include "timer.h"

const std::string ULTRA_BG_IMAGE = "/Users/Feysal/Flocks/images/ultra.png";

class Ultra : public Game
{
private:
	Timer timer;
	bool gameOver;
	
public:
	Ultra(sf::RenderWindow *renderArea) : Game(renderArea) { gameOver = false; timer.reset(); }
	
	void handleGameOverInput();
	
	void showInfos();
	
	void render();
	void play();
};

#endif
