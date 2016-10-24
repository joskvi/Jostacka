#ifndef _ITEMTEXTURES_H
#define _ITEMTEXTURES_H

#include <SFML\Graphics.hpp>
#include "PowerUp.h"

class ItemTextures
{
public:
	ItemTextures()
	{
		kSpeedUp.loadFromFile("kSpeedUp.png");
		kSpeedDown.loadFromFile("kSpeedDown.png");
	}
	sf::Texture getTexture(PowerUp::Kind kind)
	{
		switch (kind)
		{
		case PowerUp::kSpeedUp:
			return kSpeedUp;
			break;
		case PowerUp::kSpeedDown:
			return kSpeedDown;
			break;
		}
	}

private:


	sf::Texture kSpeedUp;
	sf::Texture kSpeedDown;
};

#endif