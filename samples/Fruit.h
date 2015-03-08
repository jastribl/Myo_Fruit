#include "Point.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>

using namespace std;

class Fruit:public sf::Sprite {

public:
	bool isBomb = false;
	double  xSpeed, ySpeed, rSpeed, rotation;
	bool dead = false, split = false;
	int type;
	Fruit(bool bomb, int typeGiven, sf::Texture& tex) {
		isBomb = bomb;
		type = typeGiven;
		setPosition(rand() % 1366, 868);
		xSpeed = rand() % 5 * ((getPosition().x > (1366 / 2)) ? -1 : 1);
		ySpeed = -18;
		rSpeed = rand() % 5 * ((rand() % 2 == 1) ? -1 : 1);
		rotation = 0;
		setTexture(tex);
	}

	void fly() {
		if (!dead) {
			ySpeed += 0.2f;
			rotation += rSpeed;
			setPosition(getPosition().x + xSpeed, getPosition().y + ySpeed);
			setRotation(rotation);
		}
	}

	void draw(sf::RenderWindow& window) {
		if (!dead)
			window.draw(*this);
	}
};