#ifndef GAME_H
#define GAME_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <sstream>

#include "board.h"
#include "timer.h"

enum 
{
	RUNNING = 0,
	PAUSED = 1
};

const int BLOCK_SIZE = 20;

const std::string BLOCK_FILE = "/Users/Feysal/Flocks/images/block.png";
const std::string BG_FILE = "/Users/Feysal/Flocks/images/interface.png";
const std::string PAUSE_FILE = "/Users/Feysal/Flocks/images/pause.png";

const std::string DIGITAL_POLICE = "/Users/Feysal/Flocks/fonts/digital_7/digital-7(italic).ttf";

class Game
{
	
protected:
	sf::RenderWindow *renderArea;
	
private:
	Board gameArea;
	
	
	BPiece currentGamePiece, nextPiece, holdPiece;
	
	int state;
	int linesCompleted;
	int level;
	float fallIterationDelay;
	int score;
	
	bool firstTimeHolding;
	
	std::string toString(int n)
	{
		std::ostringstream result;
		
		result << n;
		
		return result.str();
	}
	
public:
	Game() { firstTimeHolding = true; }
	Game(sf::RenderWindow *r) { firstTimeHolding = true; renderArea = r; }
	~Game() { delete renderArea; }
	
	void setCurrentGamePiece(BPiece p) { currentGamePiece = p; }
	BPiece getCurrentGamePiece() { return currentGamePiece; }
	
	void setNextPiece(BPiece p) { nextPiece = p; }
	BPiece getNextPiece() { return nextPiece; }
	
	void setHoldPiece(BPiece p) { holdPiece = p; }
	BPiece getHoldPiece() { return holdPiece; }
	
	void setState(int s) { state = s; }
	int getState() { return state; }
	
	void pause() { setState(PAUSED); }
	
	void setLinesCompleted(int l) { linesCompleted = l; }
	int getLinesCompleted() { return linesCompleted; }
	
	void setLevel(int l) { level = l; }
	int getLevel() { return level; }
	
	void setFallIterationDelay(float delay) { fallIterationDelay = delay; }
	float getFallIterationDelay() { return fallIterationDelay; }
	
	void setScore(int s) { score = s; }
	int getScore() { return score; }
	
	int computeLevel();
	float computeFallIterationDelay();
	int computeScore(int nbLinesDeleted);
	
	BPiece createNewPiece();
	
	void enableCurrentPiece();
	
	void restoreHoldPiece();
	void holdCurrentPiece();
	
	void updateGameInfos(int nbLinesDeleted);
	
	void setBackground(const std::string backgroundImage);
	void loadImage(const std::string fileName);
	
	void showScore();
	void showLevel();
	void showLinesCompleted();
	void showPiece(BPiece p, int x, int y);
	
	void showNextPiece();
	void showHoldPiece();
	
	void handlePieceLandAction();
	void handleTimerInput(float currentTime, float &precTime);
	void handleUserInput();
	void handlePauseInput();
	
	virtual void render(const std::string backgroundImage);
	
	virtual void play();
	
};

#endif