#include "modes.h"

void Sprint::render()
{
	Game::render(SPRINT_BG_IMAGE);

	timer.showTime(renderArea, 50.0, 225.0);
}

void Sprint::play()
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