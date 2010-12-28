// !!!:Feysal:20101220  
// !!!:Feysal:20101221 
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "game.h"

int main()
{
	//sf::RenderWindow *renderArea = new sf::RenderWindow(sf::VideoMode(BLOCK_SIZE * BOARD_WIDTH, BLOCK_SIZE * BOARD_HEIGHT), "Tetris");
	
	sf::RenderWindow *renderArea = new sf::RenderWindow(sf::VideoMode(BLOCK_SIZE * 30, BLOCK_SIZE * 25), "Flocks");
	renderArea->SetFramerateLimit(60);
	
	Game g(renderArea);

	g.play();
	
	return EXIT_SUCCESS;
}