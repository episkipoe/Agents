#include <vector>
#include <common/point.h>
#include <graphics/graphics.h>
#include "input.h"
#include "manage_agents.h"
#include "announcements.h"

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
		agents[i]->draw();
	}
	draw_announcements();

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
  	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,w,h);
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
		eye.z -= delta;
		if(eye.z<0) eye.z = 0;
		look.z -= delta;
		if(look.z<0) look.z = 0;
	}
	if(out) {
		eye.z += delta;
		look.z += delta;
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




