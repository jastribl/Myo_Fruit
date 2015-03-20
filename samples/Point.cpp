#include "Point.h"

Point::Point (double xGiven, double yGiven) :x (xGiven), y (yGiven) {

};

double Point::getX () {
	return x;
}

double Point::getY () {
	return y;
}

void Point::setX (double xGiven) {
	x = xGiven;
}

void Point::setY (double yGiven) {
	y = yGiven;
}

void Point::addX (double xGiven) {
	x += xGiven;
}

void Point::addY (double yGiven) {
	y += yGiven;
}