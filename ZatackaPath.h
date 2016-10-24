#ifndef _ZATACKAPATH_H
#define _ZATACKAPATH_H

#include "PowerUp.h"
#include "PowerUpManager.h"

class ZatackaPath : public sf::Drawable, public sf::Transformable
{

public:
	enum Direction { FORWARD, LEFT, RIGHT };

	ZatackaPath();
	ZatackaPath(int w, int h);
	ZatackaPath(int w, int h, sf::Color color);

	ZatackaPath& operator=(const ZatackaPath& rhs) {

		windowWidth = rhs.windowWidth;
		windowHeight = rhs.windowHeight;
		pathColor = rhs.pathColor;

		return *this;
	}

	void initializePath();
	void setHead();
	void setColor(sf::Color color);
	void setWindow(int w, int h);
	sf::Vector2f getPosition() { return position; }
	bool getCollisionState();

	void addPowerUp(PowerUp pu);
	void clearPowerUp(PowerUp pu);
	void updatePowerUps();

	void updatePath(Direction dir);
	void updatePathMatrix(int **pathMatrix);
	bool checkCollisions(int **pathMatrix);
	PowerUp::Kind checkPowUpCollisions(PowerUpManager& manager);

private:

	// Window values
	int windowWidth, windowHeight;
	int frameCount = 0;

	// Path transformation constants
	const int pathLengthMin = 180;
	const int pathLengthMax = 230;
	int pathLength = 180;
	const float jumpLength = 1.5 * 10 / 6.0;
	const float angle = 2.3 * 3.14159 / 180.0;

	// Players path values
	float speed = 1.5;
	float thickness = 6;
	float headRadius = thickness / 2;

	// Player states
	bool wallCollision = false;
	bool pathCollision = false;
	bool collision = false;
	bool jump = false;

	// Power Ups
	std::vector<PowerUp> powerUps;

	// Create head
	sf::CircleShape head;
	sf::Vector2f headVector;

	// Create path primitive
	std::vector<sf::Vertex> m_vertices;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Color pathColor;

	// Draw function
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Apply the entety's transform
		states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

		// Draw the vertex array and the circle shape
		target.draw(&m_vertices[0], m_vertices.size(), sf::TrianglesStrip, states);
		target.draw(head);
	}

};

#endif _ZATACKAPATH_H