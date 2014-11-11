#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

struct Point  //stores a location
{
	float x;
	float y;
	float z;

	Point() {
		x = 0; y = 0; z = 0;
	}
	Point(float theX, float theY, float theZ = 0.0) {
		x = theX; y = theY; z = theZ;
	}
	void show(void) {
		printf("%g, %g, %g\n", x, y, z);
	}
	bool inRange(const Point &rhs, float range) {
		return (distance(rhs)<range);
	}
	float distance(const Point &rhs) {
		return(sqrt(pow(x-rhs.x,2)+pow(y-rhs.y,2)+pow(z-rhs.z,2)));
	}
	float angle(Point *A, Point *C) {
		float a = this->distance(*C);
		float b = A->distance(*C);
		float c = this->distance(*A);
		float cosB = (pow(a,2) + pow(c,2) - pow(b,2)) / (2.0 * a * c);
		//printf("\t\ta %g b %g c %g : %g / %g\n", a, b, c, (pow(a,2) + pow(c,2) - pow(b,2)), (2.0 * a * c));
		return acos(cosB);
	}
	bool operator==(const Point &rhs) {
		return ((x == rhs.x) &&
			(y == rhs.y));
	}
	bool operator!=(const Point &rhs) {
		return !(*this==rhs);
	}
	Point operator=(const Point &rhs) {
		this->x=rhs.x; this->y=rhs.y; this->z=rhs.z;
		return *this;
	}
	Point operator+(const Point &rhs) {
		return Point(x+rhs.x, y+rhs.y, z+rhs.z);
	}

	Point operator*(float factor) {
		return Point(x*factor, y*factor, z*factor);
	}
	friend istream& operator>>(istream &is, Point &p) ;
	friend ostream& operator<<(ostream &os, const Point &p) ;

};

inline istream& operator>>(istream &is, Point &p) {
	is.read((char*)&p.x, sizeof(float));
	is.read((char*)&p.y, sizeof(float));
	is.read((char*)&p.z, sizeof(float));
	return is;
}
inline ostream& operator<<(ostream &os, const Point &p) {
	os.write((char*)&p.x, sizeof(float));
	os.write((char*)&p.y, sizeof(float));
	os.write((char*)&p.z, sizeof(float));
	return os;
}

#endif
