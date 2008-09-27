#ifndef MANAGE_AGENTS_H
#define MANAGE_AGENTS_H 
#include "agent.h"

Agent * add_agent(char * filename, Agent * mother=(Agent*)0) ;
Agent * get_agent_by_port(int port) ;

void draw_agents(void) ;
void remove_dead_agents(void) ;
void kill_all_agents(void) ;

void transmit_senses(void) ;

#endif

