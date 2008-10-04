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

int get_agents_by_distance(Point * eye, float maxDistance, vector<Agent*> &out_vec) {
	int agents_added=0;
	for (unsigned int i=0;i<agents.size();i++) {
		float curDistance = eye->distance(agents[i]->attr.location);
		if(curDistance<maxDistance) {
			//printf("%i is in range dx %g\n", agents[i]->get_port(), curDistance);
			agents_added++;
			out_vec.push_back(agents[i]);
			agents[i]->sort_value = curDistance;
		}
	}
	return agents_added;
}

bool agent_less (Agent *agentA, Agent *agentB) {
	//printf("cmp %i to %i %g to %g\n", agentA->get_port(), agentB->get_port(), agentA->sort_value , agentB->sort_value);
	return (agentA->sort_value < agentB->sort_value);
}

char * get_vision_vector(Agent * curAgent, int * length) {
	Point * eye = curAgent->get_location();

	float slopeToView=tan(curAgent->attr.heading); /*rise over run on unit circle*/

	/*	
	printf("get vision of %i\n\t", curAgent->get_port());
	eye->show();
	printf("looking at %g slope %g\n", curAgent->attr.heading, slopeToView);
	*/

	vector<Agent*> nearby;
	get_agents_by_distance(eye, curAgent->attr.view_distance, nearby);

	if(nearby.size()<=0) {
		*length=0;
		return (char*)0;
	}

	vector <Agent *>::iterator iter;
	for(iter = nearby.begin() ; iter!=nearby.end() ; iter++) {
		Point * target = (*iter)->get_location();
		if((*iter)->get_port() == curAgent->get_port()) {
			//printf("erase %i\n", (*iter)->get_port());
			iter = nearby.erase(iter);
			if(iter==nearby.end()) break;
			continue;
		}

		double slopeToTarget = (target->y - eye->y) / (target->x - eye->x);
		double tanAngle = ((slopeToView-slopeToTarget)/(1 + slopeToView*slopeToTarget));
		/*
		printf("compare to %i\n\t", (*iter)->get_port());
		target->show();
		printf("slope is %g\ntanAngle is %g\n", slopeToTarget, tanAngle);
		*/
		float angleToTarget;
		if(!isfinite(tanAngle)) {
			angleToTarget=PI*0.5;			
		} else {
			angleToTarget=atan(tanAngle);
		}
		//printf("angle is %g\n", angleToTarget);
		float angle = curAgent->attr.view_angle;
		if(fabs(angleToTarget) > (angle*0.5)) {
			//printf("erase %i\n", (*iter)->get_port());
			iter = nearby.erase(iter);
			if(iter==nearby.end()) break;
			continue;
		} 
		//printf("distance from %i to %i is %g\n", curAgent->get_port(), (*iter)->get_port(), (*iter)->sort_value);
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
	for(unsigned int i=0;i<nearby.size();i++) {
		int port = nearby[i]->get_port();
		//printf("\ttransmit %i %g\n", port, nearby[i]->sort_value);
		memcpy(loadingZone, &port, sizeof(int));
		loadingZone+=sizeof(int);
		memcpy(loadingZone, &nearby[i]->sort_value, sizeof(float));
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

