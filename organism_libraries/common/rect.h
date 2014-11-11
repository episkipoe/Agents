#ifndef RECT_H
#define RECT_H

#include "point.h"

struct Rect {
	Rect() {}
	Rect(float x1_, float y1_, float w_, float h_): x1(x1_), y1(y1_), w(w_), h(h_) { }
	float x1, y1;
	float w, h;	

	void fromPoints(const Point & p1, const Point &p2); 
	void translate(float x, float y) { x1+=x; y1+=y; }
	void scale(double s) { w*=s; h*=s; }

	bool overlaps(const Rect & o) {
		float x2=x1+w;
		float y2=y1+h;
		if(o.x1>x2) return false;
		if(o.y1>y2) return false;
		if(o.x1+w<x1) return false;
		if(o.y1+h<y1) return false;
		return true;
	}

	double nearestPoints(const Rect & o, Point &mine, Point &other);
};

#endif
