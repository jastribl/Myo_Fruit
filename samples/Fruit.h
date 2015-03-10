#include "Point.h"
#include <SFML/Graphics.hpp>

class Fruit:public sf::Sprite {
private:
	bool bomb = false, dead = false, split = false;
	Point velocity;
	double rotationSpeed, rotation;
	int type;

public:
	Fruit(bool bombGiven, int typeGiven, sf::Texture& tex) {
		bomb = bombGiven;
		type = typeGiven;
		setPosition(rand() % 1366, 868);
		velocity = Point((1 + rand() % 6) * ((getPosition().x > (1366 / 2)) ? -1 : 1), -18);
		rotationSpeed = rand() % 5 * ((rand() % 2 == 1) ? -1 : 1);
		rotation = 0.0;
		setTexture(tex);
	}

	bool isBomb() {
		return bomb;
	}

	bool isDead() {
		return dead;
	}

	bool isSplit() {
		return split;
	}

	Point getVelocity() {
		return velocity;
	}

	int getXVelocity() {
		return velocity.getX();
	}

	int getYVelocity() {
		return velocity.getY();
	}

	double getRotation() {
		return rotation;
	}

	int getType() {
		return type;
	}

	void setBomb(bool bombGiven) {
		bomb = bombGiven;
	}

	void setDead(bool deadGiven) {
		dead = deadGiven;
	}

	void setSplit(bool splitGiven) {
		split = splitGiven;
	}

	void setVelocity(Point velocityGiven) {
		velocity = velocityGiven;
	}

	void setVelocity(int xGiven, int yGiven) {
		velocity.setX(xGiven);
		velocity.setY(yGiven);
	}

	void setXVelocity(int xGiven) {
		velocity.setX(xGiven);
	}

	void setYVelocity(int yGiven) {
		velocity.setY(yGiven);

	}

	void setRotation(double rotationGiven) {
		rotation = rotationGiven;
	}

	void setType(int typeGiven) {
		type = typeGiven;
	}

	void fly() {
		if (!dead) {
			velocity.addY(0.2f);
			rotation += rotationSpeed;
			setPosition(getPosition().x + velocity.getX(), getPosition().y + velocity.getY());
			setRotation(rotation);
		}
	}

	void draw(sf::RenderWindow& window) {
		if (!dead)
			window.draw(*this);
	}
};