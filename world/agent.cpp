#include <sys/types.h>
#include <signal.h>
#include <common/randomhelp.h>
#include <graphics/graphics.h>
#include "manage_agents.h"

extern float text_size;

Agent::Agent(Genome * g, int mypid, int myport):genome(g), pid(mypid), port(myport) {
	attr.energy=10;
	attr.status = OK;
	attr.max_speed=3.0;
	attr.location.x=numberBetween(-10,10);
	attr.location.y=numberBetween(-10,10);
	attr.heading=numberBetween(-PI*0.5,PI*0.5);
	attr.view_angle=PI/4.0;
	attr.view_distance=80;
	memset(&avatar, 0, sizeof(Appearance));
}

Agent::~Agent() {
	printf("destruct\n");
	GLuint id = get_port();
	glDeleteTextures(1, &id);
	delete genome;
}

void Agent::set_appearance(char * data, int data_size) {
	int width, height; 
	memcpy(&width, data, sizeof(int));
	data+=sizeof(int);  data_size-=sizeof(int);
	memcpy(&height, data, sizeof(int));
	data+=sizeof(int);  data_size-=sizeof(int);
	//printf("recover image %i by %i  (%i vs %i)\n", width, height, size, data_size);

	delete [] avatar.data;
	avatar.width = width;
	avatar.height = height;
	avatar.data = new char[data_size];
	memcpy((void*)avatar.data, data, data_size); 
}

void Agent::get_appearance(int &width, int &height, char **data) {
	width = avatar.width;
	height = avatar.height;
	*data = avatar.data;
}

void Agent::draw(void) {
	int id = get_port();
	int width=avatar.width, height=avatar.height;
	float x_delta=width*0.1; 
	float y_delta=height*0.1; 
	char * data = avatar.data;
	Point * curPos = &attr.location;

	if(!glIsTexture(id)) {
		int size = width*height;
		if(size<=0) {
			glColor3f(1.0,1.0,1.0);
			stringstream lbl;  lbl<<id;
			drawText(curPos->x-x_delta, curPos->y+y_delta, lbl.str().c_str(), text_size);
			return ;
		}

		glBindTexture(GL_TEXTURE_2D, id);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	} 
	float left   = -x_delta;
	float right  =  x_delta;
	float bottom = -y_delta;
	float top    =  y_delta;

	glDepthFunc(GL_LESS);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, id);
	glPushMatrix();
	float angle = attr.heading*180.0/PI;
	glTranslatef(curPos->x, curPos->y, curPos->z);
	glRotatef(angle, 0, 0, 1);
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
	glDepthFunc(GL_ALWAYS);
	glColor3f(1.0,1.0,1.0);
	stringstream lbl;  lbl<<id;
	drawText(curPos->x-x_delta, curPos->y+y_delta, lbl.str().c_str(), text_size);

}

int Agent::kill(int politely) {
	int sig=SIGKILL;
	if(politely) sig=SIGTERM;
	return ::kill(pid, sig);
}


int Agent::sleep(void) {
	attr.status = ASLEEP;
	return ::kill(pid, SIGSTOP);
}

int Agent::wake(void) {
	attr.status = OK;
	return ::kill(pid, SIGCONT);
}

int Agent::have_child(char * filename) {
	if(attr.energy<=2) return 0;
	attr.energy*=0.5;
	printf("child %s gets %i\n", filename, attr.energy);
	Agent * child = add_agent(filename, this);
	if(child) {
		child->attr.energy=attr.energy;
		child->attr.location = attr.location;
		child->attr.location.y-=avatar.height*0.2;
	}
	return 1;
}

void Agent::attack (Agent * target) {
}

void Agent::move (int x, int y) {
	if(attr.energy<=2) {
		if(!success(50)) {
			sleep();
		}
		return;
	}
	attr.location.x += attr.max_speed*(x/double(SCHAR_MAX));
	attr.location.y += attr.max_speed*(y/double(SCHAR_MAX));
	//printf("move by %i, %i  (%g)\n", x, y, (attr.max_speed*(x/double(SCHAR_MAX))));
}

void Agent::turn (int angle) {
	if(attr.energy<=2) {
		if(!success(50)) {
			sleep();
		}
		return;
	}
	float delta = 2*PI;	
	attr.heading += -PI + (angle/double(SCHAR_MAX))*delta;
}

void Agent::end_turn(void) {
	attr.age++;
	switch(attr.status) {
		case OK: 
			break;
		case ASLEEP:
			attr.energy++;
			break;
	}
}
