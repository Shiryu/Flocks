#ifndef TIMER_H
#define TIMER_H

#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const std::string DIGITAL_FONT = "/Users/Feysal/Flocks/fonts/digital_7/digital-7.ttf";

class Timer
{
	
private:
	enum { STARTED, PAUSED };
	
	sf::Clock clock;
	int state;
	float elapsedTime;
	
	void reverse(std::string &str)
	{
		int l = str.size();
		int i = 0;
		int j = l - 1;
		
		while(i < j)
		{
			char temp = str[i];
			str[i] = str[j];
			str[j] = temp;
			i++;
			j--;
		}
	}
		
public:
	std::string toString(int n)
	{
		std::string result;
		
		do
			result.push_back((n % 10) + '0');
		while((n /= 10) != 0);
		
		reverse(result);
		
		return result;
	}

	
	void start()
	{
		if(state != STARTED)
		{
			clock.Reset();
			state = STARTED;
		}
	}
	
	void pause()
	{
		if(state != PAUSED)
		{
			elapsedTime += clock.GetElapsedTime();
			state = PAUSED;
		}
	}
	
	void reset()
	{
		clock.Reset();
		pause();
		elapsedTime = 0.0;
	}
	
	float getTime()
	{
		float time;
		
		if(state == PAUSED)
			time = elapsedTime;
		else
			time = clock.GetElapsedTime() + elapsedTime;
		
		return time;
	}
	
	int getMinutes()
	{
		return (int)getTime() / 60;
	}
	
	int getSeconds()
	{
		return (int)getTime() % 60;
	}
	
	std::string format()
	{
		int minutes = getMinutes();
		int seconds = getSeconds();
		
		std::string strMinutes, strSeconds;
		
		if(minutes >= 10)
			strMinutes = toString(minutes);
		else
		{
			strMinutes.push_back('0');
			strMinutes += toString(minutes);
		}
		
		if(seconds >= 10)
			strSeconds = toString(seconds);
		else
		{
			strSeconds.push_back('0');
			strSeconds += toString(seconds);
		}
		
		std::string result = strMinutes + " : ";
		result += strSeconds;
		
		return result;
	}
		
	void showTime(sf::RenderWindow *renderArea, float x, float y)
	{
		sf::Font digital;
		
		if(digital.LoadFromFile(DIGITAL_FONT))
		{
			sf::String time;
			time.SetText(format());
			time.SetFont(digital);
			time.SetSize(40);
			time.SetColor(sf::Color::Black);
			time.SetPosition(x, y);
			
			renderArea->Draw(time);
		}
	}
	
};

#endif