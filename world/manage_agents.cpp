#include <vector>
#include <genome/genome.h>
#include "agent.h"

extern vector <Agent *> agents;
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

Agent * get_agent(int port) {
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


