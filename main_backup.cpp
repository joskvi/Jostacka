/*#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "ZatackaPath.h"

// Tanker om endringer ved overføring til ZatackaPath-klassen:
//
// * Fjern konstantene wallCollision og pathCollision og bruk kun konstanten collision
// * Variabelen jumpLength kan ikke være konstant, men en funksjon av speed og thickness.
//   Evt må jumpLength uttrykkes i pixler, og tegningen må foregå på annet vis.
// * Restart funksjonalitet kan og brukes som konstrultør
// * Exception-handling mtp pathPosition matrisen og kalling utenfor området?


int main()
{

	// Set window settings
	const int windowWidth = 800;
	const int windowHeight = 600;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 20;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Zatacka", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	// Game constants and states
	const int pathLengthMin = 180;
	const int pathLengthMax = 230;
	int pathLength = 180;
	const int jumpLength = 10;
	const float angle = 2.3 * 3.14159 / 180.0;
	bool pause = false;

	// Player values and states
	float speed = 1.5;
	const float thickness = 6;
	const float headRadius = thickness / 2;
	bool wallCollision = false;
	bool pathCollision = false;
	bool collision = false;
	bool jump = false;
	int framCount = 0;

	// Create path primitive
	sf::Color pathColor(0, 229, 255);
	sf::Vector2f position(400, 300);
	sf::Vector2f velocity(1, 0);
	std::vector<sf::Vertex> path;
	int ** pathPositions;
	pathPositions = new int*[windowWidth];

	for (int i = 0; i < windowWidth; i++)
		pathPositions[i] = new int[windowHeight];

	for (int i = 0; i < windowWidth; i++)
	for (int j = 0; j < windowHeight; j++)
		pathPositions[i][j] = 0;

	// Create head
	sf::CircleShape head(headRadius);
	head.setFillColor(sf::Color::Yellow);
	head.setOrigin(sf::Vector2f(headRadius, headRadius));
	head.setPosition(position);
	sf::Vector2f headVector(velocity.x, velocity.y);

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
			else if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
				speed = static_cast<int>(event.key.code) - 26;
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
			{
				for (int i = 0; i < windowWidth; i++)
				for (int j = 0; j < windowHeight; j++)
					pathPositions[i][j] = 0;
				path = std::vector<sf::Vertex>();
				framCount = 0;
			}
		}

		// Transform path if not paused
		if (!pause)
		{

			// Check keyboard input and change velocity vector accordingly
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				velocity.x = velocity.x * cos(angle) + velocity.y * sin(angle);
				velocity.y = -velocity.x * sin(angle) + velocity.y * cos(angle);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				velocity.x = velocity.x * cos(angle) - velocity.y * sin(angle);
				velocity.y = velocity.x * sin(angle) + velocity.y * cos(angle);
			}

			// Adjust length of velocity to 1 (may be updated for efficency)
			float velocityLength = sqrt(pow(velocity.x, 2.0) + pow(velocity.y, 2.0));
			velocity.x /= velocityLength;
			velocity.y /= velocityLength;

			// Update position
			position.x += speed * velocity.x;
			position.y += speed * velocity.y;
			head.setPosition(position);
			headVector = sf::Vector2f(velocity.x * headRadius, velocity.y * headRadius);

			// Wall collision detection
			if ((position.x + headRadius) > windowWidth || (position.x - headRadius) < 0 || (position.y + headRadius) > windowHeight || (position.y - headRadius) < 0)
				wallCollision = true;
			else if (wallCollision)
				wallCollision = false;

			// Path collision detection
			if (!wallCollision && pathPositions[static_cast<int>(position.x + headVector.x)][static_cast<int>(position.y + headVector.y)] == 1 && !jump)
				pathCollision = true;
			else if (pathCollision)
				pathCollision = false;

			collision = wallCollision || pathCollision;

			// Update pathPosition matrix
			if (framCount < pathLength != wallCollision)
			{
				int r = 2;
				for (int i = -headRadius + r; i <= headRadius - r; i++)
				for (int j = -headRadius + r; j <= headRadius - r; j++)
					pathPositions[static_cast<int>(position.x + i)][static_cast<int>(position.y + j)] = 1;
			}
			else if (framCount < pathLength + jumpLength && !wallCollision)
			{
				jump = true;
				pathColor.a = 0;
			}
			else
			{
				jump = false;
				pathColor.a = 255;
				pathLength = pathLengthMin + rand() % (pathLengthMax - pathLengthMin);
				framCount = 0;
			}
			framCount++;

			// Update path primitive
			path.push_back(sf::Vertex(sf::Vector2f(position.x + velocity.y * headRadius, position.y - velocity.x * headRadius), pathColor));
			path.push_back(sf::Vertex(sf::Vector2f(position.x - velocity.y * headRadius, position.y + velocity.x * headRadius), pathColor));

		}

		// Draw screen
		window.clear(sf::Color::Black);
		window.draw(&path[0], path.size(), sf::TrianglesStrip);
		window.draw(head);
		window.display();

		// Output collision state
		if (collision)
		{
			std::cout << "Collision!" << std::endl;
			pause = true;
		}
		else
			std::cout << std::endl;


	} // End window loop

	// Destroy pathPositions matrix
	if (pathPositions != NULL)
	{
		for (int i = 0; i < windowWidth; i++)
			delete[] pathPositions[i];

		delete[] pathPositions;
	}

	return 0;
}*/