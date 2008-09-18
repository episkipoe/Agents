#include <GL/glut.h>
#include <vector>
#include <common/point.h>
#include <graphics/graphics.h>
#include "input.h"
#include "manage_agents.h"

Point eye;
Point look;
float heading = 0.0, speed=0.2;
float text_size=0.01;

extern vector <Agent *> agents;

void display(void) {
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);
	glColor3f(1.0,1.0,1.0); glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	for (unsigned int i=0;i<agents.size();i++) {
		int id = agents[i]->get_port();
		/*
			int width, height;
			char * data;
			agents[i]->getAppearance(width, height, &data);
			int size = width*height;
			if(size<=0) continue;
			glRasterPos3f(curPos->x, curPos->y, curPos->z);
			glDrawPixels(width, height, GL_RGBA, GL_BYTE, data);
		*/

		int width, height;
		char * data;
		agents[i]->getAppearance(width, height, &data);

		if(!glIsTexture(id)) {
			int size = width*height;
			if(size<=0) continue;
			glBindTexture(GL_TEXTURE_2D, id);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_BYTE, data);
		} 
		float x_delta=width*0.1; 
		float y_delta=height*0.1; 
		float left   = -x_delta;
		float right  =  x_delta;
		float bottom = -y_delta;
		float top    =  y_delta;

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, id);
		Point * curPos = &agents[i]->attr.location;
		glPushMatrix();
		glTranslatef(curPos->x, curPos->y, curPos->z);
		glBegin (GL_QUADS);
			glTexCoord2f (0.0f,0.0f); /* lower left corner */
			glVertex3f (left, bottom, 0.0f);
			glTexCoord2f (1.0f, 0.0f); /* lower right corner */
			glVertex3f (right, bottom, 0.0f);
			glTexCoord2f (1.0f, 1.0f); /* upper right corner */
			glVertex3f (right, top, 0.0f);
			glTexCoord2f (0.0f, 1.0f); /* upper left corner */
			glVertex3f (left, top, 0.0f);
		glEnd ();	
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glColor3f(1.0,1.0,1.0);
		stringstream lbl;  lbl<<id;
		drawText(curPos->x-x_delta, curPos->y+y_delta, lbl.str().c_str(), text_size);
	}

  	glutSwapBuffers();
}

void set_eye(void) {
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, 0.0, 1.0, 0.0);

	glutPostRedisplay();
}

void reshape(int w, int h) {
   	glMatrixMode(GL_PROJECTION) ;
   	glLoadIdentity();
	gluPerspective(45, w / h, 0.2, 1000);
	set_eye();
}

void initialize_display(void) {
	int width=640, height=480;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH) ;
	glutInitWindowSize(width,height) ;
	glutCreateWindow("The World") ;
	/*callbacks*/
	glutDisplayFunc(display) ;
	glutReshapeFunc(reshape) ;
	input::register_callbacks();
	/*get ready to draw*/
	glClearColor(0.0f,0.0f,0.0f,1.0f); 
	glEnable(GL_DEPTH_TEST);
  	glDepthFunc(GL_LESS);
  	glPointSize(4.0);
	glLineWidth(2.0);
	eye.z=100;

	reshape(width,height);
}

void move_eye(bool left, bool right, bool up, bool down, bool in, bool out) {
	float delta = log(eye.z+2.0)*speed;
	if(left) {
		eye.x -= delta;
		look.x -= delta;
	}
	if(right) {
		eye.x += delta;
		look.x += delta;
	}
	if(up) {
		eye.y += delta;
		look.y += delta;
	}
	if(down) {
		eye.y -= delta;
		look.y -= delta;
	}
	if(in) {
		eye.z -= speed;
		if(eye.z<0) eye.z = 0;
		look.z -= speed;
		if(look.z<0) look.z = 0;
	}
	if(out) {
		eye.z += speed;
		look.z += speed;
	}
	int any=left+right+up+down+in+out;
	if(!any) return ;
	set_eye();
}

void reset_eye(void) {
	eye.x=eye.y=0.0; 
	eye.z=100.0;
	look.x=look.y=look.z=0.0;
	set_eye();
}




