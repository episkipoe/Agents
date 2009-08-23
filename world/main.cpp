#include <GL/glut.h>
#include <vector>
#include <signal.h>
#include <stdlib.h>
#include <messages/message.h>
#include "agent.h"
#include "manage_agents.h"
#include "display.h"
#include "input.h"
#include "senses.h"
#include "message_dispatch.h"

int myPort=49152;
int sock; 
vector <Agent *> agents;
bool is_paused=false;

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
		glutTimerFunc(100,  timer, val);  
		input::timer();
		glutPostRedisplay();
		return;
	} 
	glutTimerFunc(200,  timer, val);  
	if(is_paused) return;
	try {
		remove_dead_agents();
		transmit_senses();
		check_for_message();
	}
	catch (...) {
		printf("world error in timer\n");
	}
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
		for(int i=0;i<4;i++) {
			add_agent(argv[gidx]);
		}
	}

	/*go to glut*/
	glutInit(&argc,argv);
	initialize_display();
	glutTimerFunc(200,  timer, MSG_TIMER);  
	glutTimerFunc(100, timer, DISPLAY_TIMER);  
	glutMainLoop();

	close (sock);
	return 1;
}
