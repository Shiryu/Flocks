#ifndef GAME_H
#define GAME_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <sstream>
#include <algorithm>

#include "board.h"

enum 
{
	RUNNING = 0,
	PAUSED = 1
};

const int N = NB_KINDS * 2;

const int BLOCK_SIZE = 20;

const std::string BLOCK_IMG = "images/block.png";
const std::string BG_IMG = "images/interface.png";
const std::string PAUSE_IMG = "images/pause.png";
const std::string GAME_OVER_IMG = "images/gameover.png";
const std::string DIGITAL_FONT = "fonts/digital_7/digital-7(italic).ttf";
const std::string VADOR_FONT = "fonts/vador.ttf";
const std::string MYRIAD_FONT = "/Library/Fonts/MyriadPro-Regular.otf";
const std::string MARATHON_DATA_FILE = "data/marathon.flk";

class Game
{
	
protected:
	sf::RenderWindow *renderArea;
	sf::Image blockImage, bgImage, gameOverImage;
	sf::Font vador, digital, myriad;
	
	std::string toString(int n)
	{
		std::ostringstream result;
		
		result << n;
		
		return result.str();
	}
	
private:
	Board gameArea;
	BPiece currentGamePiece, nextPiece, holdPiece;
	
	int state;
	int linesCompleted;
	int level;
	float fallIterationDelay;
	int score;
	
	bool firstTimeHolding;
	
	int randomBag[N];
	int currentPieceIndex;
	
	void initRandomBag();
	void shuffleRandomBag();
	
public:
	virtual void loadRessources();
	
	Game();
	Game(sf::RenderWindow *r);
	~Game();
	
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
	
	void showScore();
	void showLevel();
	void showLinesCompleted();
	void showPiece(BPiece p, int x, int y);
	
	virtual void recordBestScore();
	virtual void showBestScore();
	
	void showNextPiece();
	void showHoldPiece();
	
	virtual void showInfos();
	
	virtual void restart();
	virtual bool gameOver();
	
	void handlePieceLandAction();
	void handleTimerInput(float currentTime, float &precTime);
	void handleUserInput();
	virtual void handlePauseInput();
	virtual void handleGameOverInput();
	
	virtual void render(const std::string backgroundImage);
	
	virtual void play();
	
};

#endif