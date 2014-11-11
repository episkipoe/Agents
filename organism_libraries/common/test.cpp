#include "rect.h"

int main () {
	Rect A(10,10,10,10);
	Rect B(100,40,5,5);
	Point near_a, near_b;
	A.nearestPoints(B, near_a, near_b);
	printf("=> %g, %g and %g, %g\n", near_a.x, near_a.y, near_b.x, near_b.y);
	return 0;
}
