#ifndef POINT_H
#define POINT_H

class Point {

public:
	Point (double xGiven, double yGiven);
	double getX ();
	double getY ();
	void setX (double xGiven);
	void setY (double yGiven);
	void addX (double xGiven);
	void addY (double yGiven);

private:
	double x, y;
};

#endif