#include <vector>
#include <messages/message.h>
#include "manage_agents.h"

extern vector <Agent *> agents;
extern int myPort;

char * get_vision_vector(Agent * curAgent, int * length) {
	Point * eye = curAgent->get_location();

	float slopeToView=tan(curAgent->attr.heading); /*rise over run on unit circle*/

	int curPort = curAgent->get_port();
	/*
	printf("get vision of %i\n\t", curPort);
	eye->show();
	printf("looking at %g slope %g\n", curAgent->attr.heading, slopeToView);
	*/

	vector<Agent*> nearby;
	get_agents_by_distance(eye, curAgent->attr.view_distance, nearby);

	if(nearby.size()<=0) {
		*length=0;
		return (char*)0;
	}
	int idx=0, max=nearby.size();
	vector <Agent *>::iterator iter;
	for(iter = nearby.begin() ; iter!=nearby.end() ; ) {
		Point * target = (*iter)->get_location();
		
		if((*iter)->get_port() == curAgent->get_port()) {
			//printf("erase by identity %i\n", (*iter)->get_port());
			iter = nearby.erase(iter);
			if(iter==nearby.end()) break;
			continue;
		}

		double slopeToTarget = (target->y - eye->y) / (target->x - eye->x);
		double tanAngle = ((slopeToView-slopeToTarget)/(1 + slopeToView*slopeToTarget));
		float angleToTarget;
		if(!isfinite(tanAngle)) {
			angleToTarget=PI*0.5;			
		} else {
			angleToTarget=atan(tanAngle);
		}
		float angle = curAgent->attr.view_angle;
		if(curPort == 49153) {
			printf("%i of %i compare to %i\n", ++idx, max, (*iter)->get_port());
			target->show();
			printf("slope is %g\ntanAngle is %g\n", slopeToTarget, tanAngle);
			printf("%s angle is %g vs %g\n", (fabs(angleToTarget) > angle)? "N" : "Y", angleToTarget, angle);
		}
		if(fabs(angleToTarget) > angle) {
			iter = nearby.erase(iter);
			if(iter==nearby.end()) break;
			continue;
		} 
		//printf("distance from %i to %i is %g\n", curAgent->get_port(), (*iter)->get_port(), (*iter)->sort_value);
		iter++;
	}
	//printf("%i agents visible\n", nearby.size());
	int size_per_agent = sizeof(int)+sizeof(float);
	int size = nearby.size()*size_per_agent;
	*length=size;
	if(size<=0) { 
		return (char*)0;
	}
	sort(nearby.begin(), nearby.end(), agent_less);
	char * data = new char[size];
	char * loadingZone = data;
	for(iter = nearby.begin() ; iter!=nearby.end() ; iter++) {
		int port = (*iter)->get_port();
		//printf("\ttransmit %i %g\n", port, nearby[i]->sort_value);
		memcpy(loadingZone, &port, sizeof(int));
		loadingZone+=sizeof(int);
		float dx = (*iter)->sort_value;
		memcpy(loadingZone, &dx, sizeof(float));
		loadingZone+=sizeof(float);
	}
	return data;
}

void transmit_senses(void) {
	for (unsigned int i=0;i<agents.size();i++) {
		int message_length;
		char * data = get_vision_vector(agents[i], &message_length);
		send_message(myPort, agents[i]->get_port(), VISION, message_length, data);
		delete [] data;
	}
}

