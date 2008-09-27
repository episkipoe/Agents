#include <GL/glut.h>
#include <vector>
#include <signal.h>
#include <messages/message.h>
#include "agent.h"
#include "manage_agents.h"
#include "display.h"
#include "input.h"

int myPort=49152;
int sock; 
vector <Agent *> agents;

int check_for_message(void) {
	static char * data=0;
	int message_length, sender_port;
	if(!data) data = new char[MAXMSGSIZE+1];
	MessageType type = get_message(sock, &data, message_length, &sender_port);

	if(type==NO_MESSAGE) return 0;
	printf("world recvd %i from port %i\n", type, sender_port);

	Agent * sender = get_agent_by_port(sender_port);
	switch(type) {
		/* broadcast to all agents */
		case SOUND: 
			for (unsigned int i=0;i<agents.size();i++) {
				send_message(sender_port, agents[i]->get_port(), type, message_length, data);
			}
			break;
		case NEW_ORGANISM:
			{
				char * filename = getStringFromData(data, message_length);
				if(sender) sender->haveChild(filename);
				delete [] filename;
				send_message(myPort, sender_port, type, 0, (char*)0);
			}
			break;
		case KILL:
			{
				int victim;
				memcpy(&victim, data, sizeof(int));
				if(victim==myPort) {
					kill_all_agents();
				} else {
					Agent * target = get_agent_by_port(victim);
					if(sender && target) {
						sender->attack(target);
					}	
				}
			}
			break;
		case VISION:
			printf("%i receieved VISION\n", sender_port);
			if(sender) sender->setAppearance(data, message_length);
			break;
		case MOVE:
			{
				char x, y;
				memcpy(&x, data, sizeof(char));
				data+=sizeof(char);
				memcpy(&y, data, sizeof(char));
				printf("move x,y %i,%i\n", x, y);
				sender->move(x, y);
			}	
			break;
		default:
			break;
	}

	return 1;
}

void handle_signal (int signum) {
	if(signum == SIGTERM) {
		kill_all_agents();
		exit(0);
	}
} 

#define MSG_TIMER 0
#define DISPLAY_TIMER 1
void timer(int val) {
	if(val==DISPLAY_TIMER) {
		input::timer();
		glutPostRedisplay();
		glutTimerFunc(10,  timer, val);  
		return;
	} 
	try {
		remove_dead_agents();
		transmit_senses();
		check_for_message();
	}
	catch (...) {
		printf("world error in timer\n");
	}
	glutTimerFunc(5,  timer, val);  
}

int main (int argc, char * argv[]) {
	sock = UDP_bind(myPort);
	printf("world started on port %i\n", myPort);

	struct sigaction new_action;
	new_action.sa_handler = handle_signal;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags=0;
	sigaction(SIGTERM, &new_action, NULL);

	for(int gidx = 1 ; gidx<argc ; gidx++) {
		for(int i=0;i<10;i++) {
			add_agent(argv[gidx]);
		}
	}

	/*go to glut*/
	glutInit(&argc,argv);
	initialize_display();
	glutTimerFunc(5,  timer, MSG_TIMER);  
	glutTimerFunc(10, timer, DISPLAY_TIMER);  
	glutMainLoop();

	close (sock);
	return 1;
}
