#include <GL/freeglut.h>
#include "string.h"
#include "graphics.h"
#include "text.h"


void drawText(float x, float y, const std::string & text, const Font & font) {
	return drawText(x,y,0.0,text,font);
}
void drawText(float x, float y, float z, const std::string & text, const Font & font) {
	glPointSize(1.0);
	glLineWidth(1.0);
  	glPushMatrix();
  	glTranslatef(x, y, z);
  	glScalef(font.size, font.size, font.size);

	void *fontID=GLUT_STROKE_ROMAN;

	if(font.outline) {
		double y0=0.0,y1=glutStrokeHeight(fontID);
		double x0=0.0,x1=0.0;
		for(size_t i = 0; i < text.length(); ++i) x1+=glutStrokeWidth(fontID,text[i]);
		glBegin(GL_LINES);
		for(size_t i = 0; i < text.length(); ++i) {
			glVertex2f(x0,y0); glVertex2f(x1,y0);
			glVertex2f(x0,y1); glVertex2f(x1,y1);
			glVertex2f(x0,y0); glVertex2f(x0,y1);
			glVertex2f(x1,y0); glVertex2f(x1,y1);
		}
		glEnd();
	}

  	for(size_t i = 0; i < text.length(); ++i) {
     		glutStrokeCharacter(fontID, text[i]);
	}
  	glPopMatrix();
}

Rect boundingBox(const std::string & text, const Font & font) {
  	glPushMatrix();
  	glScalef(font.size, font.size, font.size);

	void *fontID=GLUT_STROKE_ROMAN;

	double y0=0.0,y1=glutStrokeHeight(fontID);
	double x0=0.0,x1=0.0;
	for(size_t i = 0; i < text.length(); ++i) x1+=glutStrokeWidth(fontID,text[i]);

	Rect rect;
	rect.fromPoints(Point(x0, y0), Point(x1, y1));
	rect.scale(font.size);

  	glPopMatrix();

	return rect;
}
