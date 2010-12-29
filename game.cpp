#include <iostream>
#include <fstream>
#include "game.h"

using namespace std;

ofstream fout("output.txt");

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

BPiece Game::createNewPiece()
{
	int kind = sf::Randomizer::Random(0, NB_KINDS - 1);
	//int orientation = sf::Randomizer::Random(0, NB_ROTATIONS - 1);
	
	BPiece b(Piece(kind, 0));

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
	BPiece temp = getHoldPiece();
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

void Game::setBackground()
{
	sf::Image image;
	
	if(image.LoadFromFile(BG_FILE))
	{
		sf::Sprite sprite;
		sprite.SetImage(image);
		
		sprite.SetPosition(0.0, 0.0);
		renderArea->Draw(sprite);
	}
}

void Game::loadImage(const std::string fileName)
{
	sf::Image image;
	
	if(image.LoadFromFile(fileName))
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
	
	sf::Font digital;
	
	if(digital.LoadFromFile(DIGITAL_POLICE))
	{
		sf::String currentScore;
		currentScore.SetFont(digital);
		currentScore.SetText(str);
		currentScore.SetSize(25);
		currentScore.SetColor(sf::Color::Black);
		currentScore.SetPosition(500, 250);
		
		renderArea->Draw(currentScore);
	}
}

void Game::showLevel()
{
	int l = getLevel();
	std::string str = toString(l);
	
	sf::Font font;
	
	if(font.LoadFromFile("/Library/Fonts/MyriadPro-Regular.otf"))
	{
		sf::String currentLevel;
		currentLevel.SetFont(font);
		currentLevel.SetText("Niveau : " + str);
		//currentLevel.SetText("Level : " + str);
		currentLevel.SetSize(11);
		currentLevel.SetColor(sf::Color::Black);
		currentLevel.SetPosition(430, 215);
		
		renderArea->Draw(currentLevel);
	}
}

void Game::showLinesCompleted()
{
	int n = getLinesCompleted();
	std::string str = toString(n);
	
	sf::Font myriad;
	
	if(myriad.LoadFromFile("/Library/Fonts/MyriadPro-Regular.otf"))
	{
		sf::String currentLinesCompleted;
		currentLinesCompleted.SetFont(myriad);
		currentLinesCompleted.SetText("Lignes : " + str);
		//currentLinesCompleted.SetText("Lines : " + str);
		currentLinesCompleted.SetSize(11);
		currentLinesCompleted.SetColor(sf::Color::Black);
		currentLinesCompleted.SetPosition(430, 230);
		
		renderArea->Draw(currentLinesCompleted);
	}
}

void Game::showPiece(BPiece p, int x, int y)
{
	int k = p.getKind();
	int o = p.getOrientation();
	
	sf::Image blockImage;
	
	if(blockImage.LoadFromFile(BLOCK_FILE))
	{
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

void Game::handleTimerInput(float currentTime, float &precTime)
{
	if(currentTime - precTime >= getFallIterationDelay())
	{
		if(!gameArea.isCurrentPieceFallen())
			gameArea.moveCurrentPieceDown();
		else
		{
			gameArea.moveCurrentPieceDown();
			
			setCurrentGamePiece(getNextPiece());
			enableCurrentPiece();
			setNextPiece(createNewPiece());
			
			int n = gameArea.deletePossibleLines();
			updateGameInfos(n);
						
			
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
				holdCurrentPiece();
			
			if(event.Key.Code == sf::Key::Left)
				gameArea.moveCurrentPieceLeft();
								
			if(event.Key.Code == sf::Key::Right)
				gameArea.moveCurrentPieceRight();
				
			if(event.Key.Code == sf::Key::Up)
				gameArea.rotateCurrentPieceRight();
			
			if(event.Key.Code == sf::Key::Space)
			{
				gameArea.dropCurrentPiece();	
				
				if(gameArea.isCurrentPieceFallen())
				{
					gameArea.moveCurrentPieceDown();
					
					setCurrentGamePiece(getNextPiece());
					enableCurrentPiece();
					setNextPiece(createNewPiece());
					
					int n = gameArea.deletePossibleLines();
					updateGameInfos(n);
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
			gameArea.moveCurrentPieceDown();
	
			setCurrentGamePiece(getNextPiece());
			enableCurrentPiece();
			setNextPiece(createNewPiece());
			
			int n = gameArea.deletePossibleLines();
			updateGameInfos(n);
		}
	}
		

}
	
void Game::handlePauseInput()
{
	sf::Event event;
	
	while(renderArea->GetEvent(event))
	{
		if(event.Type == sf::Event::KeyPressed)
		{
			if(event.Key.Code == sf::Key::P)
			{
				if(getState() == PAUSED)
					setState(RUNNING);
				else
					setState(PAUSED);
			}
		}
	}
}

void Game::render()
{
	renderArea->Clear(sf::Color(175, 175, 175));
	sf::Image blockImage;

	setBackground();
	
	if(getState() == PAUSED)
		loadImage(PAUSE_FILE);
	
	if(blockImage.LoadFromFile(BLOCK_FILE))
	{
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
	}
	
	showNextPiece();
	showHoldPiece();
	showScore();
	showLevel();
	showLinesCompleted();
}

void Game::play()
{
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
			handlePauseInput();
		else
		{
			currentTime = fallingClock.GetElapsedTime();
		
			handleTimerInput(currentTime, precTime);
		
			handleUserInput();
		}
		
		render();
		
		renderArea->Display();
	}
}