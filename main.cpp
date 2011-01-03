#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "game.h"
#include "mode.h"

int main(int argc, char *argv[])
{
	sf::RenderWindow *renderArea = new sf::RenderWindow(sf::VideoMode(BLOCK_SIZE * 30, BLOCK_SIZE * 25), "Flocks");
	renderArea->SetFramerateLimit(60);
	 
	 /*Ultra s(renderArea);
	 
	 s.play();*/
	 
	 /*Game g(renderArea);
	 
	 g.play();*/
	 
	Sprint s(renderArea);
	 
	s.play();
	
	return 0;
}