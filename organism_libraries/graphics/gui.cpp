#include "graphics.h"
#include "text.h"
#include <GL/gl.h>

using namespace std;

void drawButton(float bottom, float left, float width, float height, string caption, int id) 
{
	if (id==0) {
		glColor3f(0,0,0);
	} else {
		float color=0.4;
		glColor3f(color, color, color);
	}

	glBegin(GL_QUADS);
	 	glVertex3f(left, bottom + height, 0);
	 	glVertex3f(left + width, bottom + height, 0);
	 	glVertex3f(left + width, bottom, 0);
	 	glVertex3f(left, bottom, 0);
	glEnd();

	glColor3f(1,1,1);

	drawText(left + 1.0, bottom + (height / 2), caption, Font());
}
