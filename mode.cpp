#include "mode.h"

void Ultra::restart()
{
	timer.reset();
	Game::restart();
}

void Ultra::showInfos()
{
	showNextPiece();
	showHoldPiece();
	showScore();
	showLevel();
	showLinesCompleted();
	showBestScore();
}

bool Ultra::timeElapsed()
{
	if(timer.getTime() > 120.0)
		return true;
	
	return false;
}

void Ultra::recordBestScore()
{
	int lastBestScore = 0;
	
	std::ifstream fin(ULTRA_DATA_FILE.c_str());
	
	fin >> lastBestScore;
	
	fin.close();
	
	if(getScore() > lastBestScore)
	{
		std::ofstream fout(ULTRA_DATA_FILE.c_str());
		
		fout << getScore() << std::endl;
		
		fout.close();
	}
}

void Ultra::showBestScore()
{
	std::fstream bs(ULTRA_DATA_FILE.c_str(), std::fstream::in | std::fstream::out);
	
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

void Ultra::handlePauseInput()
{
	pauseMusic();
	
	setBackground(PAUSE_IMG);
	
	sf::Event event;
	
	while(renderArea->GetEvent(event))
	{
		if(event.Type == sf::Event::Closed)
			renderArea->Close();
		
		if(event.Type ==  sf::Event::KeyPressed)
		{
			if(event.Key.Code == sf::Key::P)
				setState(RUNNING);
			
			if(event.Key.Code == sf::Key::R)
				restart();
		}
	}
}

void Ultra::handleGameOverInput()
{
	Ultra::recordBestScore();
	
	if(Game::gameOver())
		setBackground(GAME_OVER_IMG);
	
	if(timeElapsed())
		setBackground(TIME_ELAPSED_IMG);
	
	sf::String level, lines, score;
	
	level.SetText(toString(getLevel())); lines.SetText(toString(getLinesCompleted())); score.SetText(toString(getScore()));
	
	level.SetFont(vador); lines.SetFont(vador); score.SetFont(vador);
	
	level.SetSize(24.0); lines.SetSize(24.0); score.SetSize(24.0);
	
	level.SetPosition(270.0, 175.0); lines.SetPosition(270.0, 205.0); score.SetPosition(270.0, 230.0);
	
	renderArea->Draw(level); renderArea->Draw(lines); renderArea->Draw(score);
	
	sf::Event event;
	
	while(renderArea->GetEvent(event))
	{
		if(event.Type == sf::Event::Closed)
			renderArea->Close();
		
		if(event.Type == sf::Event::KeyPressed)
		{
			if(event.Key.Code == sf::Key::R)
				restart();
		}
	}
}

void Ultra::render()
{
	Game::render(ULTRA_BG_IMG);
	showInfos();
	timer.showTime(renderArea, 50.0, 225.0);
}

void Ultra::play()
{
	playMusic();
	
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
			timer.pause();
			handlePauseInput();
		}
		else
		{
			timer.start();
			currentTime = fallingClock.GetElapsedTime();
			
			handleTimerInput(currentTime, precTime);
			
			handleUserInput();
			
			if(Game::gameOver() || timeElapsed())
				handleGameOverInput();
			else
				render();
		}
		
		renderArea->Display();
	}
}


void Sprint::restart()
{
	nbLinesToDelete = NB_LINES_TO_DELETE;
	timer.reset();
	Game::restart();
}

bool Sprint::finished()
{
	if(getNbLinesToDelete() == 0)
		return true;
	
	return false;
}

void Sprint::recordBestScore()
{
	float lastBestTime;
	
	std::ifstream fin(SPRINT_DATA_FILE.c_str());
	
	fin >> lastBestTime;
	
	fin.close();
	
	if(finished() && (timer.getTime() < lastBestTime))
	{
		std::ofstream fout(SPRINT_DATA_FILE.c_str());
		
		fout << timer.getTime() << std::endl;
		
		fout.close();
	}
}

void Sprint::showBestScore()
{
	std::fstream bs(SPRINT_DATA_FILE.c_str(), std::fstream::in | std::fstream::out);
	
	float bestTime;
	
	bs >> bestTime;
	
	std::string result;
	
	result = timer.format(bestTime);
	
	/*std::string minuts, colons, seconds;
	bs >> minuts;
	bs >> colons;
	bs >> seconds;
	
	std::string bestTime;
	bestTime = minuts + " ";
	bestTime += colons;
	bestTime += " ";
	bestTime += seconds;*/
	
	sf::String bScore;
	bScore.SetText(result);
	bScore.SetFont(vador);
	bScore.SetSize(24);
	bScore.SetColor(sf::Color(158, 14, 64));
	bScore.SetPosition(300.0, 10.0);
	
	renderArea->Draw(bScore);
	
	bs.close();
}


void Sprint::showInfos()
{
	showNextPiece();
	showHoldPiece();
	showBestScore();
	
	int n = getNbLinesToDelete();
	
	std::string str;
	
	if(n >= 0)
		str = timer.toString(n);
	else
		str = "0";
	
	sf::String currentLinesCompleted;
	currentLinesCompleted.SetFont(digital);
	currentLinesCompleted.SetText(str);
	currentLinesCompleted.SetSize(40);
	currentLinesCompleted.SetColor(sf::Color::Black);
	currentLinesCompleted.SetPosition(500.0, 240.0);
		
	renderArea->Draw(currentLinesCompleted);
}

void Sprint::handlePauseInput()
{
	pauseMusic();
	
	setBackground(PAUSE_IMG);
	
	sf::Event event;
	
	while(renderArea->GetEvent(event))
	{
		if(event.Type == sf::Event::Closed)
			renderArea->Close();
		
		if(event.Type ==  sf::Event::KeyPressed)
		{
			if(event.Key.Code == sf::Key::P)
				setState(RUNNING);
			
			if(event.Key.Code == sf::Key::R)
				restart();
		}
	}
}

void Sprint::handleGameOverInput()
{
	recordBestScore();
	
	if(Game::gameOver())
	{
		setBackground(SPRINT_GAMEOVER_IMG);
		
		sf::String lines, time;
		
		lines.SetFont(vador);
		lines.SetText(toString(getLinesCompleted()) + " / 40");
		
		time.SetFont(vador);
		time.SetText(timer.format());
		
		lines.SetSize(30.0);
		time.SetSize(30.0);
		
		lines.SetPosition(300.0, 187.0);
		time.SetPosition(300.0, 222.0);
		
		renderArea->Draw(lines);
		renderArea->Draw(time);
	}
	
	if(finished())
	{
		setBackground(SPRINT_FINISHED_IMG);
		
		sf::String time;
		
		time.SetText(timer.format());
		time.SetFont(vador);
		time.SetSize(48.0);
		
		time.SetPosition(245.0, 205.0);
		
		renderArea->Draw(time);
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
		}
	}
}

void Sprint::render()
{
	Game::render(SPRINT_BG_IMG);
	showInfos();
	timer.showTime(renderArea, 50.0, 225.0);
}


void Sprint::play()
{
	playMusic();
	
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
		setNbLinesToDelete(NB_LINES_TO_DELETE - getLinesCompleted());
		
		if(getState() == PAUSED)
		{
			timer.pause();
			handlePauseInput();
		}
		else
		{
			timer.start();
			currentTime = fallingClock.GetElapsedTime();
				
			handleTimerInput(currentTime, precTime);
				
			handleUserInput();
				
			if(Game::gameOver() || finished())
			{
				timer.pause();
				handleGameOverInput();
			}
			else
				render();
		}
		
		renderArea->Display();
	}
	
}