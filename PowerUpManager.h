#ifndef _POWERUPMANAGER_H
#define _POWERUPMANAGER_H

#include "PowerUp.h"

class PowerUpManager : public sf::Transformable, public sf::Drawable
{

public:
	PowerUpManager(int width, int height);

	void update();
	void reset();
	PowerUp::Kind checkCollision(int x, int y, float headRadius);

private:
	int windowWidth;
	int windowHeight;
	const int numPowUps = 4;

	int radius = 25;
	sf::Texture textures[4];
	struct powerUpItem {
		PowerUp::Kind puKind;
		sf::Sprite m_sprite;
	};
	std::vector<powerUpItem> m_items;

	sf::Time spawnTime;
	const int spawnTimeMin = 5;
	const int spawnTimeMax = 6;
	sf::Clock time;

	// Spawn powerUp
	void spawnPowerUp();

	// Draw function
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Apply the entety's transform
		states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

		// Draw the m_shapes vector
		for (int i = 0; i < m_items.size(); i++)
			target.draw(m_items[i].m_sprite);
	}

};

#endif _POWERUPMANAGER_H