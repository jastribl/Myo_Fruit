#ifndef POINT_H
#define POINT_H
struct Point {

private:
	int x, y;

public:
	Point() {
		x = 0;
		y = 0;
	}

	Point(int xGiven, int yGiven) {
		x = xGiven;
		y = yGiven;
	}

	Point operator+(Point& p) {
		return Point(getX() + p.getX(), getY() + p.getY());
	}

	bool operator==(Point& p) {
		return getX() == p.getX() && getY() == p.getY();
	}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	void setX(int xGiven) {
		x = xGiven;
	}

	void setY(int yGiven) {
		y = yGiven;
	}

	void addX(int xMod) {
		x += xMod;
	}

	void addY(int yMod) {
		y += yMod;
	}
};

#endif