#include <sys/types.h>
#include <signal.h>
#include <common/randomhelp.h>
#include "manage_agents.h"

Agent::Agent(Genome * g, int mypid, int myport):genome(g), pid(mypid), port(myport) {
	attr.energy=10;
	attr.status = OK;
	attr.max_speed=3.0;
	attr.location.x=numberBetween(-10,10);
	attr.location.y=numberBetween(-10,10);
	attr.heading=0.0;
	attr.view_angle=PI/4.0;
	attr.view_distance=80;
	memset(&avatar, 0, sizeof(Appearance));
}

Agent::~Agent() {
	printf("destruct\n");
	delete genome;
}

void Agent::setAppearance(char * data, int data_size) {
	int width, height; 
	memcpy(&width, data, sizeof(int));
	data+=sizeof(int);  data_size-=sizeof(int);
	memcpy(&height, data, sizeof(int));
	data+=sizeof(int);  data_size-=sizeof(int);
	int size = width*height;
	printf("recover image %i by %i  (%i vs %i)\n", width, height, size, data_size);
	if(data_size<size) return ;

	delete [] avatar.data;
	avatar.width = width;
	avatar.height = height;
	avatar.data = new char[size];
	memcpy((void*)avatar.data, data, size); 
}

void Agent::getAppearance(int &width, int &height, char **data) {
	width = avatar.width;
	height = avatar.height;
	*data = avatar.data;
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

int Agent::haveChild(char * filename) {
	if(attr.energy<=2) return 0;
	attr.energy*=0.5;
	printf("child %s gets %i\n", filename, attr.energy);
	Agent * child = add_agent(filename, this);
	if(child) {
		child->attr.energy=attr.energy;
		child->attr.location = attr.location;
		child->attr.location.y-=1.0;
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
	printf("move by %i, %i  (%g)\n", x, y, (attr.max_speed*(x/double(SCHAR_MAX))));
}

void Agent::endTurn(void) {
	attr.age++;
	switch(attr.status) {
		case OK: 
			break;
		case ASLEEP:
			attr.energy++;
			break;
	}
}
