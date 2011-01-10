#ifndef GAME_H
#define GAME_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "fmod.h"
#include "fmod_errors.h"

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "board.h"
#include "config.h"


enum  { RUNNING, PAUSED};

const int BLOCK_SIZE = 20;

const int RANDOMBAG_SIZE = NB_KINDS * 2;

const std::string BLOCK_IMG = "images/block.png";
const std::string BG_IMG = "images/interface.png";
const std::string PAUSE_IMG = "images/pause.png";
const std::string GAME_OVER_IMG = "images/gameover.png";

const std::string DIGITAL_FONT = "fonts/digital_7/digital-7(italic).ttf";
const std::string VADOR_FONT = "fonts/vador.ttf";
const std::string MYRIAD_FONT = "/Library/Fonts/MyriadPro-Regular.otf";

const std::string HOLD_SOUND = "sounds/hold.wav";
const std::string MOVE_SOUND = "sounds/move.wav";
const std::string DROP_SOUND = "sounds/drop.wav";
const std::string DELETE_SOUND = "sounds/delete.wav";

const std::string MARATHON_MUSIC = "sounds/Korobeiniki.mid";
const std::string SPRINT_MUSIC = "sounds/korobeiniki_tidwell.ogg";
const std::string ULTRA_MUSIC = "sounds/korobeiniki_ozma.ogg";

const std::string MARATHON_DATA_FILE = "data/marathon.flk";

const int CONTINUE = 0;
const int RETURN_MAIN = 1;

class Game
{
	
protected:
	sf::RenderWindow *renderArea;
	sf::Image blockImage, bgImage, gameOverImage;
	sf::Font vador, digital, myriad;
	
	FMOD_SYSTEM *system;
	FMOD_CHANNEL *channel, *musicChannel;
	FMOD_SOUND *hold, *move, *drop, *deletion;
	FMOD_SOUND *marathonMusic, *sprintMusic, *ultraMusic;
	
	inline std::string toString(int n)
	{
		std::ostringstream result;
		result << n;
		return result.str();
	}
	
private:
	Config config;
	
	Board gameArea;
	Piece currentGamePiece, nextPiece, holdPiece;
	
	int state;
	int linesCompleted;
	int level;
	float fallIterationDelay;
	int score;
	
	bool firstTimeHolding;
	
	int randomBag[RANDOMBAG_SIZE];
	int currentPieceIndex;
	
	void initRandomBag();
	void shuffleRandomBag();
	
public:
	virtual void loadRessources();
	
	Game();
	Game(sf::RenderWindow *r);
	~Game();
	
	void playSound(FMOD_SOUND *sound);
	void playMusic(FMOD_SOUND *music);
	
	void pauseMusic();
	void continueMusic();
	
	void setCurrentGamePiece(Piece p) { currentGamePiece = p; }
	Piece getCurrentGamePiece() { return currentGamePiece; }
	
	void setNextPiece(Piece p) { nextPiece = p; }
	Piece getNextPiece() { return nextPiece; }
	
	void setHoldPiece(Piece p) { holdPiece = p; }
	Piece getHoldPiece() { return holdPiece; }
	
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
	
	Piece createNewPiece();
	
	void enableCurrentPiece();
	
	void restoreHoldPiece();
	void holdCurrentPiece();
	
	void updateGameInfos(int nbLinesDeleted);
	
	void setBackground(const std::string backgroundImage);
	
	void showScore();
	void showLevel();
	void showLinesCompleted();
	void showPiece(Piece p, int x, int y);
	
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
	
	virtual int handlePauseInput();
	virtual int handleGameOverInput();
	
	virtual void render(const std::string backgroundImage);
	
	virtual void play();
	
};

#endif