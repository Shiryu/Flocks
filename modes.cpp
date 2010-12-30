#include "modes.h"

void Ultra::showInfos()
{
	showNextPiece();
	showHoldPiece();
	//showScore();
	//showLevel();
	
	int n = getLinesCompleted();
	std::string str = timer.toString(n);
	
	sf::Font digital;
	
	if(digital.LoadFromFile(DIGITAL_FONT))
	{
		sf::String currentLinesCompleted;
		currentLinesCompleted.SetFont(digital);
		currentLinesCompleted.SetText(str);
		currentLinesCompleted.SetSize(40);
		currentLinesCompleted.SetColor(sf::Color::Black);
		currentLinesCompleted.SetPosition(500.0, 240.0);
		
		renderArea->Draw(currentLinesCompleted);
	}
}

void Ultra::render()
{
	Game::render(ULTRA_BG_IMAGE);
	showInfos();
	timer.showTime(renderArea, 50.0, 225.0);
}

void Ultra::play()
{
	renderArea->Clear(sf::Color(245, 245, 200));
	
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
		if(timer.getTime() <= 120.0)
		{
			if(getState() == PAUSED)
			{
				handlePauseInput();
				timer.pause();
			}
			else
			{
				timer.start();
				currentTime = fallingClock.GetElapsedTime();
			
				handleTimerInput(currentTime, precTime);
			
				handleUserInput();
			}
		
			render();
		}
		else
		{
			if(!gameOver)
			{
				setBackground("/Users/Feysal/Flocks/images/timefinish.png");
				gameOver = true;
			}
		}
		
		renderArea->Display();
	}
}