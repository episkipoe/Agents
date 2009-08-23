#include <vector>
#include <algorithm>
#include <genome/genome.h>
#include <messages/message.h>
#include "manage_agents.h"

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


