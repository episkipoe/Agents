#include <QFileInfo>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/wait.h>
#include <genome/genome.h>
#include <messages/message.h>
#include "manage_agents.h"
#include "announcements.h"

extern vector <Agent *> agents;

Agent * add_agent(const char * filename) {
	static int curPort=49153;

	QFileInfo file_info(filename);
	if(!file_info.exists()) return (Agent*)0;

	ifstream validateStream(filename, ios::in);
	char magic[8];
	validateStream.getline(magic, 8);
	if(strcmp(magic, "//body")) return (Agent*)0;

	Genome * myGenome;
	myGenome = new Genome(filename);
	Agent * newAgent;
	char portNum[256]; sprintf(portNum, "%i", curPort); 
	pid_t pid = myGenome->buildOrganismFromGenome(portNum);
	if(pid<0 || waitpid(pid, (int*)0, WNOHANG)>0) return (Agent*)0;
 
	newAgent = new Agent(myGenome, pid, curPort++);
	agents.push_back(newAgent);
	printf("loaded %s on port %i (%p)\n", filename, newAgent->get_port(), newAgent);
	return newAgent;
}

template <class A> struct name_equal : public binary_function<A,char *,bool> {
	bool operator() (const A&agent, const char *name) const { return (!strcmp(agent->getName().c_str(),name)); }
};

Agent * get_agent_by_name(const char * name) {
	agent_iter iter;
	iter = find_if(agents.begin(), agents.end(), bind2nd(name_equal<Agent*>(), name));
	if(iter == agents.end()) return (Agent*)0;
	return *iter;
}

template <class A> struct port_equal : public binary_function<A,int,bool> {
	bool operator() (const A&agent, const int&port) const { return (agent->get_port() == port); }
};

Agent * get_agent_by_port(int port) {
	agent_iter iter;
	iter = find_if(agents.begin(), agents.end(), bind2nd(port_equal<Agent*>(), port));
	if(iter == agents.end()) return (Agent*)0;
	return *iter;
}

template <class A> struct pid_equal : public binary_function<A,pid_t,bool> {
	bool operator() (const A&agent, const pid_t&pid) const { return (agent->get_pid() == pid); }
};

bool is_dead(Agent * curAgent) { return (curAgent->attr.energy < 0); }

void remove_dead_agents(void) {
	for(agent_iter iter=agents.begin(); iter != agents.end() ; iter++) {
		if(is_dead(*iter)) {
			(*iter)->kill(0);
			printf("kill %s\n", (*iter)->getName().c_str());
		}
	}

	int status; pid_t dead_pid;
	while ((dead_pid = waitpid(-1, &status, WNOHANG))>0) {
		agent_iter iter = find_if(agents.begin(), agents.end(), bind2nd(pid_equal<Agent*>(), dead_pid));
		if(iter != agents.end()) {
			Agent * deadAgent = *iter;

			stringstream msg;
			msg<<deadAgent->getName();
			switch(deadAgent->attr.status) {
				case EATEN: msg<<" has been eaten"; break;
				default: msg<<" has died"; break;
			}
			add_announcement(msg.str().c_str(), deadAgent->get_location(), 25);


			delete deadAgent;
			agents.erase(iter);
		}
	}
}

void kill_all_agents(void) {
	printf("kill_all!\n");
	for (unsigned int i=0;i<agents.size();i++) {
		agents[i]->kill(0);
	}
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


