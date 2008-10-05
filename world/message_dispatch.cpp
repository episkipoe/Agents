#include <vector>
#include <messages/message.h>
#include "agent.h"
#include "manage_agents.h"
#include "display.h"
#include "announcements.h"
#include "food.h"

extern int myPort;
extern int sock; 
extern vector <Agent *> agents;

int check_for_message(void) {
	static char * data=0;
	char data_copy[MAXMSGSIZE+1];
	int message_length, sender_port;
	if(!data) data = new char[MAXMSGSIZE+1];
	MessageType type = get_message(sock, &data, message_length, &sender_port);
	if(type==NO_MESSAGE) return 0;
	//printf("world recvd %i from port %i\n", type, sender_port);

	Agent * sender = get_agent_by_port(sender_port);
	switch(type) {
		/* broadcast to all agents */
		case SOUND: 
			if(sender) {
				for(int i=0 ; i < message_length ; i++) {
					if(isprint(data[i])) {
						data_copy[i]=data[i];
					} else {
						data_copy[i]='.';
					}
				}
				data_copy[message_length]=0;
				add_announcement(data_copy, sender->get_location(), 20);
			}	
			for (unsigned int i=0;i<agents.size();i++) {
				send_message(sender_port, agents[i]->get_port(), type, message_length, data);
			}
			break;
		case NEW_ORGANISM:
			{
				char * filename = getStringFromData(data, message_length);
				int child_id;
				if(sender) {
					child_id = sender->have_child(filename);
					if(child_id > 0) send_message(myPort, sender_port, type, sizeof(int), (char*)&child_id);
				}
				delete [] filename;
			}
			break;
		case KILL:
			{
				int target;
				memcpy(&target, data, sizeof(int));
				if(target==myPort) {
					kill_all_agents();
				} else {
					Agent * victim = get_agent_by_port(target);
					if(sender && victim) {
						sender->attack(victim);
					}	
				}
			}
			break;
		case VISION:
			if(sender) sender->set_appearance(data, message_length);
			break;
		case MOVE:
			{
				char x, y;
				memcpy(&x, data, sizeof(char));
				data+=sizeof(char);
				memcpy(&y, data, sizeof(char));
				//printf("move x,y %i,%i\n", x, y);
				sender->move(x, y);
			}	
			break;
		case TURN:
			{
				char angle;
				memcpy(&angle, data, sizeof(char));
				//printf("turn angle %i\n", angle);
				sender->turn(angle);
			}
			break;
		case EAT:
			{
				int target;
				memcpy(&target, data, sizeof(int));
				if(sender) {
					attempt_to_eat(sender, target);
				}
			}
			break;
		default:
			printf("unhandled message %i\n", type);
			break;
	}

	return 1;
}

