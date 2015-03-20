#include "Fruit.h"

Fruit::Fruit (bool bombGiven, bool deadGiven, bool splitGiven, int typeGiven, sf::Texture& tex, double yVelocity)
	:bomb (bombGiven), dead (deadGiven), split (splitGiven), type (typeGiven),
	rSpeed (rand () % 5 * ((rand () % 2 == 1) ? -1 : 1)),
	rotation (0), velocity (0, 0) {
	setPosition (rand () % 1366, 868);
	velocity = Point ((1 + rand () % 6) * ((getPosition ().x > (1366 / 2)) ? -1 : 1), yVelocity);
	setTexture (tex);
}

bool Fruit::isBomb () { return bomb; };
bool Fruit::isSplit () { return split; };
bool Fruit::isDead () { return dead; };
Point Fruit::getVelocity () { return velocity; };
double Fruit::getXSpeed () { return velocity.getX (); };
double Fruit::getYSpeed () { return velocity.getY (); };
int Fruit::getType () { return type; }
void Fruit::setBomb (bool bombGiven) { bomb = bombGiven; }
void Fruit::setDead (bool deadGiven) { dead = deadGiven; }

void Fruit::fly () {
	if (!dead) {
		velocity.addY (0.2f);
		rotation += rSpeed;
		setPosition (getPosition ().x + velocity.getX (), getPosition ().y + velocity.getY ());
		setRotation (rotation);
	}
}

void Fruit::draw (sf::RenderWindow& window) {
	window.draw (*this);
}