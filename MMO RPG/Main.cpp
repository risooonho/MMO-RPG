#include <SFML/Graphics.hpp>

#include <windows.h>

#include "DeltaTime.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	sf::RenderWindow window{ sf::VideoMode{ Graphics::ScreenWidth, Graphics::ScreenHeight }, "MMO RPG" };

	Game game{ window };

	while (window.isOpen())
	{
		{
			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
			}
		}

		game.Main();
		
		DeltaTime::Restart();
	}

	return 0;
}