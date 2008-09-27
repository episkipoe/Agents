#include <vector>
#include <genome/genome.h>
#include <messages/message.h>
#include "agent.h"

extern vector <Agent *> agents;
extern int myPort;
int curPort=49153;

Agent * add_agent(char * filename, Agent *mother) {
	Genome * myGenome;
	myGenome = new Genome(filename);
	Agent * newAgent;
	char portNum[256]; sprintf(portNum, "%i", curPort); 
	int pid = myGenome->buildOrganismFromGenome(portNum);
	newAgent = new Agent(myGenome, pid, curPort++);
	agents.push_back(newAgent);
	printf("loaded %s on port %i (%p)\n", filename, newAgent->get_port(), newAgent);
	return newAgent;
}

template <class A> struct port_equal : public binary_function<A,int,bool> {
	bool operator() (const A&agent, const int&port) const { return (agent->get_port() == port); }
};

Agent * get_agent_by_port(int port) {
	vector <Agent *>::iterator iter;
	iter = find_if(agents.begin(), agents.end(), bind2nd(port_equal<Agent*>(), port));
	if(iter == agents.end()) return (Agent*)0;
	return *iter;
}

bool is_dead(Agent * curAgent) { return (curAgent->attr.energy < 0); }

void remove_dead_agents(void) {
	vector <Agent *>::iterator iter;
	while((iter = find_if(agents.begin(), agents.end(), is_dead)) != agents.end()) {
		Agent * curAgent = *iter;
		if(!curAgent->kill(0)) agents.erase(iter);
	}
}
void kill_all_agents(void) {
	printf("kill_all!\n");
	for (unsigned int i=0;i<agents.size();i++) {
		agents[i]->kill(0);
	}
	agents.clear();
}

int get_agents_by_distance(Point * eye, float distance, vector<Agent*> &out_vec) {
	int agents_added=0;
	for (unsigned int i=0;i<agents.size();i++) {
		if(eye->distance(agents[i]->attr.location)<distance) {
			agents_added++;
			out_vec.push_back(agents[i]);
		}
	}
	return agents_added;
}

char * get_vision_vector(Agent * curAgent, int * length) {
	*length=0;
	return (char*)0;

	Point * eye = curAgent->getLocation();
	printf("get vision of %i\n\t", curAgent->get_port());
	eye->show();

	float slopeToView=tan(curAgent->attr.heading); /*rise over run on unit circle*/
	vector<Agent*> nearby;
	get_agents_by_distance(eye, curAgent->attr.view_distance, nearby);
	printf("%i agents in range\n", nearby.size());
	vector <Agent *>::iterator iter;
	for(iter = nearby.begin() ; iter!=nearby.end() ; iter++) {
		Point * target = (*iter)->getLocation();
		if((*iter)->get_port() == curAgent->get_port()) {
			nearby.erase(iter);
		}

		printf("compare to %i\n\t", (*iter)->get_port());
		target->show();

		double slopeToTarget = (target->y - eye->y) / (target->x - eye->x);
		printf("slope is %g\n", slopeToTarget);
		double tanAngle = ((slopeToView-slopeToTarget)/(1 + slopeToView*slopeToTarget));
		printf("tanAngle is %g\n", tanAngle);
		float angleToTarget;
		if(!isfinite(tanAngle)) {
			angleToTarget=PI*0.5;			
		} else {
			angleToTarget=atan(tanAngle);
		}
		printf("angle is %g\n", angleToTarget);
		float angle = curAgent->attr.view_angle;
		if(fabs(angleToTarget) < (angle*0.5)) {
			printf("erase iter %p\n", iter);
			nearby.erase(iter);
		}
	}
	printf("%i agents in angle\n", nearby.size());

	//int size = nearby.size()*sizeof(int);
}

void transmit_senses(void) {
	for (unsigned int i=0;i<agents.size();i++) {
		int message_length;
		char * data = get_vision_vector(agents[i], &message_length);
		send_message(myPort, agents[i]->get_port(), VISION, message_length, data);
		delete [] data;
	}
}

