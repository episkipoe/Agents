#include <QApplication>  
#include <QWidget>  

#include <vector>
#include <signal.h>
#include "WorldWindow.h"
#include "WorldView.h"
#include "agent.h"
#include "manage_agents.h"

WorldWindow *window;
WorldView *world_view;
vector <Agent *> agents;

void handle_signal (int signum) {
	if(signum == SIGTERM) {
		kill_all_agents();
		exit(0);
	}
} 

int main (int argc, char * argv[]) {
	int world_port = 49152;

	struct sigaction new_action;
	new_action.sa_handler = handle_signal;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags=0;
	sigaction(SIGTERM, &new_action, NULL);

	QApplication app(argc, argv);
	window = new WorldWindow();
	world_view = new WorldView(window);	

	window->setWindowTitle("Agent World");
	window->setCentralWidget(world_view);
	window->resize(600, 400);
	window->set_port(world_port);
	window->show();

	for(int gidx = 1 ; gidx<argc ; gidx++) {
		for(int i=0;i<4;i++) {
			add_agent(argv[gidx]);
		}
	}

	return app.exec();
}
