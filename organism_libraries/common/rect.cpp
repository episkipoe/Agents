#include <math.h>
#include <vector>
#include "rect.h"

using namespace std;

void Rect::fromPoints(const Point & p1, const Point &p2) {
	float min_x = (p1.x < p2.x ? p1.x : p2.x);
	float min_y = (p1.y < p2.y ? p1.y : p2.y);
	x1 = min_x;  y1 = min_y;
	w = fabs(p2.x - p1.x);
	h = fabs(p2.y - p1.y);
}

double Rect::nearestPoints(const Rect & o, Point &mine, Point &other) {
	double min_dx; size_t min_a=0, min_b=0;
	vector<Point> a;
	vector<Point> b;
	a.push_back(Point(x1, y1));	
	a.push_back(Point(x1, y1+h));	
	a.push_back(Point(x1+h, y1));	
	a.push_back(Point(x1+h, y1+h));	
	b.push_back(Point(o.x1, o.y1));	
	b.push_back(Point(o.x1, o.y1+o.h));	
	b.push_back(Point(o.x1+o.h, o.y1));	
	b.push_back(Point(o.x1+o.h, o.y1+o.h));	

	for(size_t a_idx=0 ; a_idx<a.size() ; a_idx++) {	
		for(size_t b_idx=0 ; b_idx<b.size() ; b_idx++) {
			double dx = a[a_idx].distance(b[b_idx]);
			if(!a_idx&&!b_idx) {
				min_dx = dx;
			} else if(dx < min_dx) {
				min_dx = dx;
				min_a = a_idx;
				min_b = b_idx;
			}
		}
	}
	mine = a[min_a];
	other = b[min_b];
	return min_dx;
}

