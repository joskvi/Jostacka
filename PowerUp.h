#ifndef _POWERUP_H
#define _POWERUP_H

class PowerUp
{
public:
	enum Kind
	{
		kSpeedUp = 0,
		kSpeedDown,
		kSpeedUpOther,
		kSpeedDownOther,
		kThicknessUp,
		kThicknessDown,
		kNone
	};

	PowerUp(Kind powerUpKind);
	bool isActive();
	Kind getKind() const { return kind; }



private:
	bool active;
	Kind kind;
	sf::Time lifeTime;
	sf::Clock time;

};

#endif