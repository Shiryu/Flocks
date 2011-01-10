#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

const std::string CONFIG_FILE = "flocksrc";
const std::string SOUND = "sound";
const std::string MUSIC = "music";

const int NB_OPTIONS = 2;

class Config
{
public:
	Config() {}
	
	std::map<std::string, bool> m;
	
	void loadConfig()
	{
		int n = NB_OPTIONS;
		
		std::ifstream fin(CONFIG_FILE.c_str());
		
		while(n--)
		{
			std::string param;
			int value;
			
			fin >> param >> value;
			
			if(value == 1)
				m[param] = true;
			else
				m[param] = false;
		}
	}
};

#endif