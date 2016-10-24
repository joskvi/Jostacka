#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "ZatackaPath.h"
#include "PowerUp.h"
#include "PowerUpManager.h"


ZatackaPath::ZatackaPath()
{
	initializePath();
}

ZatackaPath::ZatackaPath(int w, int h) : windowWidth(w), windowHeight(h)
{
	initializePath();
}

ZatackaPath::ZatackaPath(int w, int h, sf::Color color) : windowWidth(w), windowHeight(h), pathColor(color)
{
	initializePath();
}

void ZatackaPath::initializePath()
{
	m_vertices = std::vector<sf::Vertex>();
	collision = false;
	powerUps.clear();

	speed = 1.5;
	thickness = 6;
	headRadius = thickness / 2;

	int minStartX = windowWidth / 6, maxStartX = windowWidth * 5 / 6;
	int minStartY = windowHeight / 6, maxStartY = windowHeight * 5 / 6;
	position.x = minStartX + rand() % (maxStartX - minStartX);
	position.y = minStartY + rand() % (maxStartY - minStartY);

	float inAng = (rand() % 360) * 3.14159 / 180.0;
	velocity = sf::Vector2f(1, 0);
	velocity.x = velocity.x * cos(inAng) + velocity.y * sin(inAng);
	velocity.y = -velocity.x * sin(inAng) + velocity.y * cos(inAng);

	head.setFillColor(sf::Color::Yellow);
	setHead();

}

void ZatackaPath::setHead()
{
	headVector = sf::Vector2f(velocity.x * headRadius, velocity.y * headRadius);
	head.setRadius(headRadius);
	head.setOrigin(sf::Vector2f(headRadius, headRadius));
	head.setPosition(position);
}

void ZatackaPath::setColor(sf::Color color)
{
	pathColor = color;
}

void ZatackaPath::setWindow(int w, int h)
{
	windowWidth = w;
	windowWidth = h;
}

bool ZatackaPath::getCollisionState()
{
	return collision;
}

void ZatackaPath::addPowerUp(PowerUp pu)
{
	powerUps.push_back(pu);
	switch (pu.getKind())
	{
	case PowerUp::kSpeedUp:
		speed *= 1.5;
		break;
	case PowerUp::kSpeedDown:
		speed /= 1.5;
		break;
	case PowerUp::kThicknessUp:
		thickness *= 2;
		break;
	case PowerUp::kThicknessDown:
		thickness /= 2;
		break;
	}
	headRadius = thickness / 2;
	setHead();
}

void ZatackaPath::clearPowerUp(PowerUp pu)
{
	switch (pu.getKind())
	{
	case PowerUp::kSpeedUp:
		speed /= 1.5;
		break;
	case PowerUp::kSpeedDown:
		speed *= 1.5;
		break;
	case PowerUp::kThicknessUp:
		thickness /= 2;
		break;
	case PowerUp::kThicknessDown:
		thickness *= 2;
		break;
	}
	headRadius = thickness / 2;
	setHead();
}

void ZatackaPath::updatePowerUps()
{
	for (int i = 0; i < powerUps.size(); )
	{
		if (!powerUps[i].isActive() && !jump)
		{
			clearPowerUp(powerUps[i]);
			powerUps.erase(powerUps.begin() + i);
		}
		else
			i++;
	}
	// Check againts ManagePowerUps here. Do something about jump as well
}

void ZatackaPath::updatePath(Direction dir)
{
	frameCount++;

	// Check keyboard input and change velocity vector accordingly
	if (dir == LEFT)
	{
		velocity.x = velocity.x * cos(angle) + velocity.y * sin(angle);
		velocity.y = -velocity.x * sin(angle) + velocity.y * cos(angle);
	}
	else if (dir == RIGHT)
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
	headVector = sf::Vector2f(velocity.x * headRadius, velocity.y * headRadius);
	head.setPosition(position);

	// Check jump
	if (frameCount % (pathLength + static_cast<int>(thickness * jumpLength / speed)) > pathLength)
	{
		//pathLength = pathLengthMin + rand() % (pathLengthMax - pathLengthMin);
		jump = true;
	}
	else
		jump = false;

	// Change alpha channel according to jump state
	if (jump)
		pathColor.a = 0;
	else
		pathColor.a = 255;

	// Update powerups
	updatePowerUps();

	// Add to m_verticies
	m_vertices.push_back(sf::Vertex(sf::Vector2f(position.x + velocity.y * headRadius, position.y - velocity.x * headRadius), pathColor));
	m_vertices.push_back(sf::Vertex(sf::Vector2f(position.x - velocity.y * headRadius, position.y + velocity.x * headRadius), pathColor));
}

void ZatackaPath::updatePathMatrix(int **pathMatrix)
{
	if (!wallCollision && !jump)
	{
		int r = 2;
		for (int i = -headRadius + r; i <= headRadius - r; i++)
			for (int j = -headRadius + r; j <= headRadius - r; j++)
				pathMatrix[static_cast<int>(position.x + i)][static_cast<int>(position.y + j)] = 1;
	}
}

bool ZatackaPath::checkCollisions(int **pathMatrix)
{
	// Wall collision detection
	if ((position.x + headRadius) >= windowWidth || (position.x - headRadius) <= 0 || (position.y + headRadius) >= windowHeight || (position.y - headRadius) <= 0)
		wallCollision = true;
	else if (wallCollision)
		wallCollision = false;

	// Wall collision detection
	if (!wallCollision && !jump && pathMatrix[static_cast<int>(position.x + headVector.x)][static_cast<int>(position.y + headVector.y)] == 1)
		pathCollision = true;
	else if (pathCollision)
		pathCollision = false;

	collision = wallCollision || pathCollision;
	if (collision)
		powerUps.clear();
	return (collision);

}

PowerUp::Kind ZatackaPath::checkPowUpCollisions(PowerUpManager& manager)
{
	return manager.checkCollision(position.x, position.y, headRadius);
}