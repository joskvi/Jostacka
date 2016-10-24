#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

#include "ZatackaPath.h"
#include "PowerUp.h"
#include "PowerUpManager.h"

int main()
{
	// Set window settings
	const int windowWidth = 1400;
	const int windowHeight = 800;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 20;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Zatacka", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	// Game states
	bool gameOver = true;
	bool pause = false;
	bool enablePowerUps = false;
	int numPlayers = 3;
	int numActivePlayers = numPlayers;

	// Create players
	std::vector<std::string> Names = { "Jostein", "Dag", "Odd", "Henrik" };
	std::vector<sf::Color> Colors = { sf::Color::Cyan, sf::Color::White, sf::Color::Magenta, sf::Color::Red };
	std::vector<sf::Keyboard::Key> Controls = {
		sf::Keyboard::Q, sf::Keyboard::A,
		sf::Keyboard::X, sf::Keyboard::C,
		sf::Keyboard::N, sf::Keyboard::M,
		sf::Keyboard::Down, sf::Keyboard::Right };
	struct player
	{
		std::string name;
		sf::Keyboard::Key left;
		sf::Keyboard::Key right;
		ZatackaPath path;
		int points = 0;
	};
	std::vector<player> players(numPlayers);
	for (int i = 0; i < numPlayers; i++)
	{
		players[i].name = Names[i];
		players[i].left = Controls[2 * i];
		players[i].right = Controls[2 * i + 1];
		players[i].path = ZatackaPath(windowWidth, windowHeight, Colors[i]);
	}

	// Path mask
	int ** pathPositions;
	pathPositions = new int*[windowWidth];
	for (int i = 0; i < windowWidth; i++)
		pathPositions[i] = new int[windowHeight];

	// Create PowerUp manager
	PowerUpManager manager(windowWidth, windowHeight);


	while (window.isOpen())
	{

		// Check key events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				pause = !pause;
		}

		// Reset game
		if (gameOver && !pause)
		{
			// Clear position mask
			for (int i = 0; i < windowWidth; i++)
				for (int j = 0; j < windowHeight; j++)
					pathPositions[i][j] = 0;

			// Initialize player paths
			for (int i = 0; i < numPlayers; i++)
			{
				players[i].path.initializePath();
				for (int j = 0; j < 10; j++)
				{
					players[i].path.updatePath(ZatackaPath::FORWARD);
					players[i].path.updatePathMatrix(pathPositions);
				}
			}

			// Reset powerUps
			manager.reset();

			// Set game states
			gameOver = false;
			pause = true;
		}

		// Change game state if not paused
		if (!pause)
		{
			// Update PowerUpManager
			manager.update();

			// Check keyboard input and transform accordingly
			for (int i = 0; i < numPlayers; i++)
			{

				if (!players[i].path.getCollisionState())
				{
					if (sf::Keyboard::isKeyPressed(players[i].left))
						players[i].path.updatePath(ZatackaPath::LEFT);
					else if (sf::Keyboard::isKeyPressed(players[i].right))
						players[i].path.updatePath(ZatackaPath::RIGHT);
					else
						players[i].path.updatePath(ZatackaPath::FORWARD);

					players[i].path.updatePathMatrix(pathPositions);
				}
			}


			for (int i = 0; i < numPlayers; i++)
			{

				// Check powerUps
				PowerUp::Kind puKind = players[i].path.checkPowUpCollisions(manager);
				if (puKind <= PowerUp::kSpeedDown)
					players[i].path.addPowerUp(puKind);
				else if (puKind < PowerUp::kThicknessUp)
				{
					int lol = static_cast<int>(puKind)-2;
					puKind = static_cast<PowerUp::Kind>(lol);
					for (int j = 0; j < numPlayers; j++)
					{
						if (j != i)
							players[j].path.addPowerUp(puKind);
					}
				}

				if (numPlayers > 1)
				{
				// Check collisions
				if (!players[i].path.getCollisionState() && players[i].path.checkCollisions(pathPositions))
				{
					// Update player points
					for (int j = 0; j < numPlayers; j++)
					{
						if (j != i && !players[j].path.getCollisionState())
							players[j].points++;
					}
					numActivePlayers--;

				}

				// Update game state
				if (numActivePlayers == 1)
				{
					gameOver = true;
					pause = true;
					numActivePlayers = numPlayers;
					// Output player points
					for (int i = 0; i < numPlayers; i++)
						std::cout << players[i].name << ": " << players[i].points << std::endl;
					std::cout << std::endl;
					break;
				}
				}
			}

		}

		// Draw screen
		window.clear(sf::Color::Black);
		for (int i = 0; i < numPlayers; i++)
			window.draw(players[i].path);
		window.draw(manager);
		window.display();


	} // End window loop

	// Destroy pathPositions matrix
	if (pathPositions != NULL)
	{
		for (int i = 0; i < windowWidth; i++)
			delete[] pathPositions[i];

		delete[] pathPositions;
	}


	return 0;
}