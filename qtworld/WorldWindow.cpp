#include <QTimer>
#include <QMenu>
#include <QMenuBar>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSignalMapper>
#include <messages/message.h>
#include "WorldWindow.h"
#include "WorldView.h"
#include "AgentList.h"
#include "senses.h"
#include "manage_agents.h"
#include "message_dispatch.h"

extern WorldView * world_view;

WorldWindow::WorldWindow(QWidget *parent) : QMainWindow(parent) { 
	is_paused=false;
	sock = -1;

	QTimer * timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(agent_timer()));
	timer->start(600);

	LifeAndDeathOptions();
	CreateMenu();

}

WorldWindow::~WorldWindow() {
	kill_all_agents();
	if(sock>=0) ::close(sock);
}

void WorldWindow::set_port(int port_no) {
	if(sock>=0) ::close(sock);
	myPort = port_no;

	sock = UDP_bind(myPort);
	if(sock == -1) return ;
	printf("world started on port %i\n", myPort);

}

void WorldWindow::agent_timer(void) {
	if(is_paused || sock < 0) return;
	try {
		remove_dead_agents();
		transmit_senses(myPort);
		check_for_message(sock, myPort);
	}
	catch (...) {
		printf("world error in timer\n");
	}
}

void WorldWindow::CreateMenu(void) {
	QMenu *menu = menuBar()->addMenu(tr("&Tools"));

	QAction *lad_action = menu->addAction(tr("&Life and Death..."));
	connect(lad_action, SIGNAL(triggered()), this, SLOT(LifeAndDeathOptions()));

	QAction *agent_action = menu->addAction(tr("&Agent Options..."));
	connect(agent_action, SIGNAL(triggered()), this, SLOT(AgentOptions()));

	QAction *display_action = menu->addAction(tr("&Display Properties..."));
	connect(display_action, SIGNAL(triggered()), this, SLOT(DisplayOptions()));

}

void WorldWindow::LifeAndDeathOptions(void) {
	QDockWidget *dock = new QDockWidget(tr("Life and Death"), this);
	addDockWidget(Qt::LeftDockWidgetArea, dock);

	QGroupBox *options = new QGroupBox(dock);
	QVBoxLayout * vbox = new QVBoxLayout;
	options->setLayout(vbox);
	dock->setWidget(options);

	QPushButton * create = new QPushButton("&Add organism", options);
	vbox->addWidget(create);
	connect(create, SIGNAL(clicked()), this, SLOT(AddOrganism()));

	QPushButton * nuke = new QPushButton("&Kill all organisms", options);
	vbox->addWidget(nuke);
	connect(nuke, SIGNAL(clicked()), this, SLOT(Nuke()));

	vbox->addStretch();
}
void WorldWindow::AddOrganism(void) {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Genome File"), "", tr("Genome Files (*.genome);;All Files (*)"));
	cout << "filename is " << fileName.toStdString() << endl;
	Agent * new_agent = add_agent(fileName.toStdString().c_str());
	InteractionOptions(new_agent);
}

void WorldWindow::Nuke(void) {
	kill_all_agents();
}

void WorldWindow::InteractionOptions(Agent * agent) {
	if(!agent) return ;

	AgentInterface *dock = new AgentInterface(agent, this);
	addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void WorldWindow::AgentOptions(void) {
	QDockWidget *dock = new QDockWidget(tr("Agent Options"), this);
	addDockWidget(Qt::LeftDockWidgetArea, dock);

	QGroupBox *options = new QGroupBox(dock);
	QVBoxLayout * vbox = new QVBoxLayout;
	options->setLayout(vbox);
	dock->setWidget(options);

	QGroupBox *prop_group = new QGroupBox(options);
	vbox->addWidget(prop_group);
	QHBoxLayout * hbox = new QHBoxLayout;
	prop_group->setLayout(hbox);
	AgentList * property_agents = new AgentList(options);
	hbox->addWidget(property_agents);
	AgentListButton * prop_btn = new AgentListButton(tr("Show &Properties"), prop_group, property_agents);
	hbox->addWidget(prop_btn);
	connect(prop_btn, SIGNAL(clicked()), this, SLOT(ShowProperties()));

	vbox->addStretch();

}
void WorldWindow::ShowProperties() {
	AgentListButton * btn = (AgentListButton *)sender(); 
	AgentList * al = btn->get_agent_list();
	std::string agent_name = al->currentText().toStdString();
	Agent * curAgent = get_agent_by_name(agent_name.c_str());
	InteractionOptions(curAgent);
}

void WorldWindow::DisplayOptions(void) {
	world_view->DisplayOptions(this);
}

void WorldWindow::keyPressEvent(QKeyEvent * event) {
	switch(event->key()) {
		case Qt::Key_P: is_paused = !is_paused; break;
		case Qt::Key_Q: delete this; exit(0);
		default: world_view->keyPressEvent(event); return ;
	}
}
void WorldWindow::keyReleaseEvent(QKeyEvent * event) {
	switch(event->key()) {
		default: world_view->keyReleaseEvent(event); return ;
	}
}

