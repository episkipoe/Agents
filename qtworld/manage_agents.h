#ifndef MANAGE_AGENTS_H
#define MANAGE_AGENTS_H 
#include <vector>
#include "agent.h"

Agent * add_agent(const char * filename) ;

Agent * get_agent_by_name(const char * name);
Agent * get_agent_by_port(int port) ;

bool agent_less (Agent *agentA, Agent *agentB) ;
int get_agents_by_distance(Point * eye, float maxDistance, vector<Agent*> &out_vec);

void draw_agents(void) ;
void remove_dead_agents(void) ;
void kill_all_agents(void) ;

#endif

