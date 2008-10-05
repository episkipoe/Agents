#include <vector>
#include <messages/message.h>
#include "announcements.h"
#include "manage_agents.h"

extern int myPort;

int attempt_to_eat(Agent * eater, int target) {
	Agent * victim = get_agent_by_port(target);
	if(victim) {
		stringstream msg;
		int eater_port = eater->get_port();
		int victim_port = victim->get_port();
		msg<<eater_port<<" is eating "<<victim_port;
		add_announcement(msg.str().c_str(), eater->get_location(), 20);

		eater->attr.energy++;
		victim->attr.energy--;

		send_message(myPort, eater_port, EAT, sizeof(int), (char*)&victim_port);
		send_message(myPort, victim_port, EAT, sizeof(int), (char*)&victim_port);
		return 1;
	}

	return 0;
}

