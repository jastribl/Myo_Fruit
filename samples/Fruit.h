#ifndef FRUIT_H
#define FRUIT_H

#include "Point.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>

using namespace std;

class Fruit :public sf::Sprite {

public:
	Fruit (bool bombGiven, bool deadGiven, bool splitGiven, int typeGiven, sf::Texture&, double yVelocity=-18);
	void fly ();
	void draw (sf::RenderWindow& window);
	bool isBomb ();
	bool isSplit ();
	bool isDead ();
	Point getVelocity ();
	double getXSpeed ();
	double getYSpeed ();
	int getType ();
	void setBomb (bool bombGiven);
	void setDead (bool deadGiven);

private:
	bool bomb, dead;
	const bool split;
	Point velocity;
	double rotation;
	const double rSpeed;
	int type;
};

#endif