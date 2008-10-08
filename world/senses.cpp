#include <vector>
#include <messages/message.h>
#include "manage_agents.h"

extern vector <Agent *> agents;
extern int myPort;

char * get_vision_vector(Agent * curAgent, int * length) {
	int curPort = curAgent->get_port();
	Point * eye = curAgent->get_location();
	float heading = curAgent->attr.heading;
	float slopeToView=tan(heading);
	Point lookingAt;
	if(fabs(heading)>PI*0.5) {
		lookingAt.x = eye->x-1.0;
		lookingAt.y = eye->y-slopeToView;
	} else {
		lookingAt.x = eye->x+1.0;
		lookingAt.y = eye->y+slopeToView;
	}
	lookingAt.z = eye->z;

	/*if(curPort == 49153) {
		printf("get vision of %i\n\t", curPort);
		eye->show();
		float heading_deg = heading*180.0/PI;
		printf("looking at %g (%g) \n\t", heading, heading_deg);
		lookingAt.show();
	}*/

	vector<Agent*> nearby;
	get_agents_by_distance(eye, curAgent->attr.view_distance, nearby);

	if(nearby.size()<=0) {
		*length=0;
		return (char*)0;
	}
	//int idx=0, max=nearby.size();
	vector <Agent *>::iterator iter;
	for(iter = nearby.begin() ; iter!=nearby.end() ; ) {
		Point * target = (*iter)->get_location();
		
		if((*iter)->get_port() == curPort) {
			//printf("erase by identity %i\n", (*iter)->get_port());
			iter = nearby.erase(iter);
			if(iter==nearby.end()) break;
			continue;
		}

		float angleToTarget = eye->angle(&lookingAt, target);
		float angle = curAgent->attr.view_angle;
		/*if(curPort == 49153) {
			printf("%i of %i compare to %i\n\t", ++idx, max, (*iter)->get_port());
			target->show();
			float angle_deg = angleToTarget*180.0/PI;
			printf("\t%s angle is %g (%g) vs %g\n", (fabs(angleToTarget) > angle)? "N" : "Y", angleToTarget, angle_deg, angle);
		}*/
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

