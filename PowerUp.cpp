#include <SFML/Graphics.hpp>

#include "PowerUp.h"

PowerUp::PowerUp(Kind powerUpKind)
{
	kind = powerUpKind;
	active = true;
	lifeTime = sf::seconds(5); // Dependent on time
	time.restart();
}

bool PowerUp::isActive()
{
	if (time.getElapsedTime() >= lifeTime)
		active = false;
	return active;
}