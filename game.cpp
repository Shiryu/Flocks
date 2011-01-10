#include "game.h"

//using namespace std;

//ofstream fout("output.txt");

void Game::initRandomBag()
{
	currentPieceIndex = 0;
	
	for(int i = 0, j = NB_KINDS; i < NB_KINDS, j < RANDOMBAG_SIZE; ++i, ++j)
	{
		randomBag[i] = i;
		randomBag[j] = j - NB_KINDS;
	}
}

void Game::shuffleRandomBag()
{
	std::random_shuffle(randomBag, randomBag + RANDOMBAG_SIZE);
}

void Game::loadRessources()
{
	blockImage.LoadFromFile(BLOCK_IMG);
	bgImage.LoadFromFile(BG_IMG);
	gameOverImage.LoadFromFile(GAME_OVER_IMG);
	digital.LoadFromFile(DIGITAL_FONT);
	vador.LoadFromFile(VADOR_FONT);
	myriad.LoadFromFile(MYRIAD_FONT);
	
	FMOD_System_Create(&system);
	
	FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
	
	FMOD_System_CreateSound(system, MOVE_SOUND.c_str(), FMOD_DEFAULT, 0, &move);
	FMOD_System_CreateSound(system, HOLD_SOUND.c_str(), FMOD_DEFAULT, 0, &hold);
	FMOD_System_CreateSound(system, DROP_SOUND.c_str(), FMOD_DEFAULT, 0, &drop);
	FMOD_System_CreateSound(system, DELETE_SOUND.c_str(), FMOD_DEFAULT, 0, &deletion);
	
	FMOD_System_CreateSound(system, MARATHON_MUSIC.c_str(), FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &marathonMusic);
	FMOD_System_CreateSound(system, SPRINT_MUSIC.c_str(), FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &sprintMusic);
	FMOD_System_CreateSound(system, ULTRA_MUSIC.c_str(), FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &ultraMusic);
	
	FMOD_Channel_SetVolume(musicChannel, 0.5);
}

Game::Game()
{
	config.loadConfig();
	loadRessources();
	
	firstTimeHolding = true;
	initRandomBag();
	shuffleRandomBag();
}

Game::Game(sf::RenderWindow *r)
{
	config.loadConfig();
	
	loadRessources();
	
	firstTimeHolding = true;
	initRandomBag();
	shuffleRandomBag();
	
	renderArea = r;
}

Game::~Game()
{
	FMOD_Sound_Release(hold);
	FMOD_Sound_Release(move);
	FMOD_Sound_Release(drop);
	FMOD_Sound_Release(deletion);
	
	FMOD_Sound_Release(marathonMusic);
	FMOD_Sound_Release(sprintMusic);
	FMOD_Sound_Release(ultraMusic);
	
	//FMOD_Channel_Stop(channel);
	//FMOD_Channel_Stop(musicChannel);
	
	FMOD_System_Close(system);
	FMOD_System_Release(system);
}

void Game::playSound(FMOD_SOUND *sound)
{
	if(config.m[SOUND] == true)
		FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, 0, &channel);
}

void Game::playMusic(FMOD_SOUND *music)
{
	if(config.m[MUSIC] == true)
	{
		FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, music, 0, &musicChannel);
		FMOD_Sound_SetLoopCount(music, -1);
	}
}

void Game::pauseMusic()
{
	FMOD_Channel_SetPaused(musicChannel, 1);
}

void Game::continueMusic()
{
	FMOD_Channel_SetPaused(musicChannel, 0);
}

int Game::computeLevel()
{
	int n = getLinesCompleted();
	
	if(n == 0)
		return 1;
	else if(n >= 1 && n <= 90)
	{
		int l = 1 + ((n - 1) / 10);
		return l;
	}
	else
		return 10;
}

float Game::computeFallIterationDelay()
{
	int l = getLevel();
	
	float delay = ((11 - l) * 0.05);
	
	return delay;
}

int Game::computeScore(int nbLinesDeleted)
{
	int level = getLevel();
	
	int score = 0;
	
	if(nbLinesDeleted == 1)
		score = 40 * (level + 1);
	if(nbLinesDeleted == 2)
		score = 100 * (level + 1);
	if(nbLinesDeleted == 3)
		score = 300 * (level + 1);
	if(nbLinesDeleted == 4)
		score = 1200 * (level + 1);
	
	return score;
}

Piece Game::createNewPiece()
{
	if(currentPieceIndex == RANDOMBAG_SIZE - 1)
	{
		currentPieceIndex = 0;
		shuffleRandomBag();
	}
	
	int kind = randomBag[currentPieceIndex++];
	
	Piece b(kind, 0);
	
	return b;
}

void Game::enableCurrentPiece()
{
	gameArea.newPiece(getCurrentGamePiece());
}

void Game::updateGameInfos(int nbLinesDeleted)
{
	setLinesCompleted(getLinesCompleted() + nbLinesDeleted);
	setLevel(computeLevel());
	setFallIterationDelay(computeFallIterationDelay());
	setScore(computeScore(nbLinesDeleted) + getScore());
}

void Game::restoreHoldPiece()
{
	Piece temp = getHoldPiece();
	setHoldPiece(getCurrentGamePiece());
	setCurrentGamePiece(temp);
	enableCurrentPiece();
}

void Game::holdCurrentPiece()
{
	gameArea.clearPiece(gameArea.getCurrentPiece());
	gameArea.clearPiece(gameArea.getCurrentGhostPiece());
	
	if(!firstTimeHolding)
		restoreHoldPiece();
	else
	{
		setHoldPiece(getCurrentGamePiece());
		setCurrentGamePiece(getNextPiece());
		enableCurrentPiece();
		firstTimeHolding = false;
	}
}

void Game::setBackground(const std::string backgroundImage)
{
	sf::Image image;
	
	if(image.LoadFromFile(backgroundImage))
	{
		sf::Sprite sprite;
		sprite.SetImage(image);
		
		sprite.SetPosition(0.0, 0.0);
		renderArea->Draw(sprite);
	}
}

void Game::showScore()
{
	int n = getScore();
	
	std::string str = toString(n);
	
	sf::String currentScore;
	currentScore.SetFont(digital);
	currentScore.SetText(str);
	currentScore.SetSize(25);
	currentScore.SetColor(sf::Color::Black);
	currentScore.SetPosition(500, 250);
		
	renderArea->Draw(currentScore);
}

void Game::showLevel()
{
	int l = getLevel();
	std::string str = toString(l);
	
	sf::String currentLevel;
	currentLevel.SetFont(myriad);
	currentLevel.SetText("Niveau : " + str);
	currentLevel.SetSize(11);
	currentLevel.SetColor(sf::Color::Black);
	currentLevel.SetPosition(430, 215);
		
	renderArea->Draw(currentLevel);
}

void Game::showLinesCompleted()
{
	int n = getLinesCompleted();
	std::string str = toString(n);
	
	sf::String currentLinesCompleted;
	currentLinesCompleted.SetFont(myriad);
	currentLinesCompleted.SetText("Lignes : " + str);
	currentLinesCompleted.SetSize(11);
	currentLinesCompleted.SetColor(sf::Color::Black);
	currentLinesCompleted.SetPosition(430, 230);
		
	renderArea->Draw(currentLinesCompleted);
}

void Game::showPiece(Piece p, int x, int y)
{
	int k = p.getKind();
	int o = p.getOrientation();
	
	sf::Sprite blockSprite;
	blockSprite.SetImage(blockImage);
		
	for(int i = 0; i < SIZE; ++i)
	{
		for(int j = 0; j < SIZE; ++j)
		{
			if(PIECES[k][o][j][i] != FREE)
			{
				switch(k)
				{
					case I:
						blockSprite.SetColor(sf::Color(43, 250, 250));
						break;
					case J:
						blockSprite.SetColor(sf::Color::Blue);
						break;
					case L:
						blockSprite.SetColor(sf::Color(237, 127, 16));
						break;
					case O:
						blockSprite.SetColor(sf::Color(247, 255, 60));
						break;
					case S:
						blockSprite.SetColor(sf::Color::Green);
						break;
					case T:
						blockSprite.SetColor(sf::Color(244, 24, 96));
						break;
					case Z:
						blockSprite.SetColor(sf::Color(255, 94, 77));
						break;
					default:
						break;
				}
					
				blockSprite.SetPosition((float)((i * BLOCK_SIZE) + x), (float)((j * BLOCK_SIZE) + y));
				renderArea->Draw(blockSprite);
			}
		}
	}
}

void Game::recordBestScore()
{
	int lastBestScore = 0;
	
	std::ifstream fin(MARATHON_DATA_FILE.c_str());
	
	fin >> lastBestScore;
	
	fin.close();
	
	if(getScore() > lastBestScore)
	{
		std::ofstream fout(MARATHON_DATA_FILE.c_str());
		
		fout << getScore() << std::endl;
		
		fout.close();
	}
}

void Game::showBestScore()
{
	std::fstream bs(MARATHON_DATA_FILE.c_str(), std::fstream::in | std::fstream::out);
	
	int bestScore = 0;
	bs >> bestScore;
	
	sf::String bScore;
	bScore.SetText(toString(bestScore));
	bScore.SetFont(vador);
	bScore.SetSize(24);
	bScore.SetColor(sf::Color(158, 14, 64));
	bScore.SetPosition(300.0, 10.0);
		
	renderArea->Draw(bScore);
	
	bs.close();
}

void Game::showNextPiece()
{
	showPiece(getNextPiece(), 460, 70.25);
}

void Game::showHoldPiece()
{
	if(!firstTimeHolding)
		showPiece(getHoldPiece(), 40, 70.25);
}

void Game::showInfos()
{
	showNextPiece();
	showHoldPiece();
	showScore();
	showLevel();
	showLinesCompleted();
	showBestScore();
}

void Game::restart()
{
	continueMusic();
	
	firstTimeHolding = true;
	
	shuffleRandomBag();
	currentPieceIndex = 0;
	
	gameArea.clear();
	
	setLinesCompleted(0);
	setLevel(1);
	setScore(0);
	setFallIterationDelay(computeFallIterationDelay());
	setState(RUNNING);
	
	setCurrentGamePiece(createNewPiece());
	setNextPiece(createNewPiece());
	
	enableCurrentPiece();
}

bool Game::gameOver()
{
	if(gameArea.isCurrentPieceFallen() && gameArea.isGameOver())
		return true;
	
	return false;
}

void Game::handlePieceLandAction()
{
	if(!gameOver())
	{
		gameArea.moveCurrentPieceDown();
		
		setCurrentGamePiece(getNextPiece());
		enableCurrentPiece();
		setNextPiece(createNewPiece());
		
		int n = gameArea.deletePossibleLines();
		
		if(n != 0)
			playSound(deletion);
		
		updateGameInfos(n);
	}
}

void Game::handleTimerInput(float currentTime, float &precTime)
{
	if(currentTime - precTime >= getFallIterationDelay())
	{
		if(!gameArea.isCurrentPieceFallen())
			gameArea.moveCurrentPieceDown();
		else
		{
			if(!gameOver())
				playSound(drop);
			handlePieceLandAction();
		}
				
		precTime = currentTime;
	}
}

void Game::handleUserInput()
{
	sf::Event event;
		
	while(renderArea->GetEvent(event))
	{
		if(event.Type == sf::Event::Closed)
			renderArea->Close();
		
		if(event.Type == sf::Event::KeyPressed)
		{
			if(event.Key.Shift)
			{
				playSound(hold);
				holdCurrentPiece();
			}
			
			if(event.Key.Control)
			{
				playSound(move);
				gameArea.rotateCurrentPieceLeft();
			}
			
			if(event.Key.Code == sf::Key::Left)
			{
				playSound(move);
				gameArea.moveCurrentPieceLeft();
			}
								
			if(event.Key.Code == sf::Key::Right)
			{
				playSound(move);
				gameArea.moveCurrentPieceRight();
			}
				
			if(event.Key.Code == sf::Key::Up)
			{
				playSound(move);
				gameArea.rotateCurrentPieceRight();
			}
			
			if(event.Key.Code == sf::Key::Space)
			{
				gameArea.dropCurrentPiece();	
				
				if(gameArea.isCurrentPieceFallen())
				{
					if(gameOver())
						handleGameOverInput();
					else
					{
						playSound(drop);
						handlePieceLandAction();
					}
				}
			}
			
			if(event.Key.Code == sf::Key::P)
			{
				if(getState() == PAUSED)
					setState(RUNNING);
				else
					setState(PAUSED);
			}
		}
	}
	
	const sf::Input &input = renderArea->GetInput();
	
	if(input.IsKeyDown(sf::Key::Down))
	{
		gameArea.moveCurrentPieceDown();
	
		if(gameArea.isCurrentPieceFallen())
		{
			if(gameOver())
				handleGameOverInput();
			else
			{
				playSound(drop);
				handlePieceLandAction();
			}
		}
	}
		

}

int Game::handlePauseInput()
{
	pauseMusic();
	
	setBackground(PAUSE_IMG);
	
	sf::Event event;
	
	while(renderArea->GetEvent(event))
	{
		if(event.Type == sf::Event::Closed)
			renderArea->Close();
		
		if(event.Type == sf::Event::KeyPressed)
		{
			if(event.Key.Code == sf::Key::P)
			{
				continueMusic();
				setState(RUNNING);
			}
			
			if(event.Key.Code == sf::Key::R)
				restart();
			
			if(event.Key.Code == sf::Key::M)
				return RETURN_MAIN;
		}
	}
	
	return CONTINUE;
}

int Game::handleGameOverInput()
{
	pauseMusic();
	
	recordBestScore();
	
	if(getLevel() == 10 && getLinesCompleted() >= 100)
		setBackground("images/congrat.png");
	else
	{
		setBackground(GAME_OVER_IMG);
	
		sf::String level, lines, score;
	
		level.SetText(toString(getLevel())); lines.SetText(toString(getLinesCompleted())); score.SetText(toString(getScore()));
		
		level.SetFont(vador); lines.SetFont(vador); score.SetFont(vador);
		
		level.SetSize(24.0); lines.SetSize(24.0); score.SetSize(24.0);

		level.SetPosition(270.0, 175.0); lines.SetPosition(270.0, 205.0); score.SetPosition(270.0, 230.0);
		
		renderArea->Draw(level); renderArea->Draw(lines); renderArea->Draw(score);
	}
	
	sf::Event event;
	
	while(renderArea->GetEvent(event))
	{
		if(event.Type == sf::Event::Closed)
			renderArea->Close();
		
		if(event.Type == sf::Event::KeyPressed)
		{
			if(event.Key.Code == sf::Key::R)
				restart();
			
			if(event.Key.Code == sf::Key::M)
				return RETURN_MAIN;
		}
	}
	
	return CONTINUE;
}

void Game::render(const std::string backgroundImage)
{
	setBackground(backgroundImage);
	
	sf::Sprite blockSprite;
	blockSprite.SetImage(blockImage);
		
	for(int i = 0; i < BOARD_WIDTH; ++i)
	{
		for(int j = 0; j < BOARD_HEIGHT; ++j)
		{
			if(gameArea.area[i][j] != FREE)
			{
				switch(gameArea.area[i][j])
				{
					case CYAN:
						blockSprite.SetColor(sf::Color(43, 250, 250));
						break;
					case BLUE:
						blockSprite.SetColor(sf::Color::Blue);
						break;
					case ORANGE:
						blockSprite.SetColor(sf::Color(237, 127, 16));
						break;
					case YELLOW:
						blockSprite.SetColor(sf::Color(247, 255, 60));
						break;
					case GREEN:
						blockSprite.SetColor(sf::Color::Green);
						break;
					case PURPLE:
						blockSprite.SetColor(sf::Color(244, 24, 96));
						break;
					case RED:
						blockSprite.SetColor(sf::Color(255, 94, 77));
						break;
					case GHOST:
						blockSprite.SetColor(sf::Color(255, 255, 255, 128));
						break;
					default:
						break;
				}
						
				blockSprite.SetPosition((float)((i * BLOCK_SIZE) + 200), (float)((j * BLOCK_SIZE) + 50));
				renderArea->Draw(blockSprite);
			}
		}
	}
	
	showInfos();
}

void Game::play()
{
	playMusic(marathonMusic);
	
	setCurrentGamePiece(createNewPiece());
	setNextPiece(createNewPiece());
	
	enableCurrentPiece();
	
	setLinesCompleted(0);
	setLevel(1);
	setFallIterationDelay(computeFallIterationDelay());
	setScore(0);
	setState(RUNNING);
	
	sf::Clock fallingClock;
	fallingClock.Reset();
	
	float currentTime = 0, precTime = 0;
	
	while(renderArea->IsOpened())
	{
		if(getState() == PAUSED)
		{
			if(handlePauseInput() == RETURN_MAIN)
				return;
		}
		else
		{
			currentTime = fallingClock.GetElapsedTime();
		
			handleTimerInput(currentTime, precTime);
		
			handleUserInput();
			
			
			if(!gameOver())
				render(BG_IMG);
			else
			{
				if(handleGameOverInput() == RETURN_MAIN)
					return;
			}
		}
				
		renderArea->Display();
	}
}