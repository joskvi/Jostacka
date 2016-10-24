#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>

#include "PowerUpManager.h"
#include "PowerUp.h"

PowerUpManager::PowerUpManager(int width, int height) : windowWidth(width), windowHeight(height)
{
	// Load texture
	textures[0].loadFromFile("kSpeedUp.png");
	textures[1].loadFromFile("kSpeedDown.png");
	textures[2].loadFromFile("kSpeedUpOther.png");
	textures[3].loadFromFile("kSpeedDownOther.png");

	// Manage time
	spawnTime = sf::seconds(spawnTimeMin + rand() % (spawnTimeMax - spawnTimeMin));
	time.restart();

}

void PowerUpManager::update()
{
	if (time.getElapsedTime() > spawnTime)
	{
		spawnPowerUp();
		time.restart();
		spawnTime = sf::seconds(spawnTimeMin + rand() % (spawnTimeMax - spawnTimeMin));
	}

}

void PowerUpManager::reset()
{
	time.restart();
	m_items.clear();
}

void PowerUpManager::spawnPowerUp()
{
	int i = rand() % numPowUps;
	PowerUp::Kind puKind = static_cast<PowerUp::Kind>(i);

	int x = radius + rand() % (windowWidth - 2 * radius);
	int y = radius + rand() % (windowHeight - 2 * radius);
	sf::Sprite sprite;
	sprite.setPosition(sf::Vector2f(x, y));
	sprite.setOrigin(sf::Vector2f(radius, radius));

	sprite.setTexture(textures[i]);
	sprite.setColor(sf::Color(255, 255, 255, 200));

	powerUpItem item;
	item.puKind = puKind;
	item.m_sprite = sprite;

	m_items.push_back(item);
}

PowerUp::Kind PowerUpManager::checkCollision(int x, int y, float headRadius)
{
	for (int i = 0; i < m_items.size(); i++)
	{
		sf::Vector2f pos = m_items[i].m_sprite.getPosition();
		if (pow((pos.x - x), 2) + pow((pos.y - y), 2) <= pow((radius + headRadius), 2))
		{
			PowerUp::Kind kind = m_items[i].puKind;
			m_items.erase(m_items.begin() + i);
			return kind;
		}
	}
	return PowerUp::kNone;
}


