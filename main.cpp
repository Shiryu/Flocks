#include "game.h"
#include "mode.h"

const std::string MENU_IMG = "images/welcome.png";
const std::string APROPOS_IMG = "images/apropos.png";

void handleAProposInput(sf::RenderWindow *renderArea)
{
	sf::Event event;
	
	while(renderArea->IsOpened())
	{
		while(renderArea->GetEvent(event))
		{
			if(event.Type == sf::Event::Closed)
				renderArea->Close();
			
			if(event.Type == sf::Event::KeyPressed)
			{
				if(event.Key.Code == sf::Key::R)
					return;
			}	
		}
		
		renderArea->Display();
	}
}

int main(int argc, char *argv[])
{
	sf::RenderWindow *renderArea = new sf::RenderWindow(sf::VideoMode(BLOCK_SIZE * 30, BLOCK_SIZE * 25), "Flocks");
	
	renderArea->SetFramerateLimit(60);

	sf::Image welcome, aPropos;
	sf::Sprite welcomeSprite, aProposSprite;
	
	welcome.LoadFromFile(MENU_IMG);
	aPropos.LoadFromFile(APROPOS_IMG);
		
	welcomeSprite.SetImage(welcome);
	welcomeSprite.SetPosition(0.0, 0.0);
		
	aProposSprite.SetImage(aPropos);
	aProposSprite.SetPosition(0.0, 0.0);
		
	renderArea->Draw(welcomeSprite);
	
	while(renderArea->IsOpened())
	{
		sf::Event event;
		
		while(renderArea->GetEvent(event))
		{
			if(event.Type == sf::Event::Closed)
				renderArea->Close();
			
			if(event.Type == sf::Event::KeyPressed)
			{
				if(event.Key.Code == sf::Key::M)
				{
					Game marathon(renderArea);
					marathon.play();
				}
				
				if(event.Key.Code == sf::Key::S)
				{
					Sprint sprint(renderArea);
					sprint.play();
				}
				
				if(event.Key.Code == sf::Key::U)
				{
					Ultra ultra(renderArea);
					ultra.play();
				}
				
				if(event.Key.Code == sf::Key::A)
				{
					renderArea->Draw(aProposSprite);
					handleAProposInput(renderArea);
				}
				
				if(event.Key.Code == sf::Key::Q)
					renderArea->Close();
			}
		}
		
		renderArea->Draw(welcomeSprite);
		renderArea->Display();
	}
	
	return 0;
}