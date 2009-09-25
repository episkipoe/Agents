#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalMapper>
#include <AgentInterface.h>
#include <messages/message.h>
#include "WorldWindow.h"


AgentInterface::AgentInterface(Agent * a, QWidget * parent, Qt::WindowFlags flags) : QDockWidget(parent, flags), agent(a) {
	QString title("");
	title+=a->getName().c_str();
	setWindowTitle(title);

	QGroupBox *options = new QGroupBox(this);
	QVBoxLayout * vbox = new QVBoxLayout;
	options->setLayout(vbox);
	setWidget(options);

	QGroupBox *say_group = new QGroupBox(options);
	vbox->addWidget(say_group);
	QHBoxLayout * hbox = new QHBoxLayout;
	say_group->setLayout(hbox);
	QLineEdit * say_txt = new QLineEdit(say_group);
	hbox->addWidget(say_txt);
	QPushButton * say_btn = new QPushButton("&Say", say_group);
	hbox->addWidget(say_btn);
	QSignalMapper * say_map = new QSignalMapper(say_group);
	connect(say_btn, SIGNAL(clicked()), say_map, SLOT(map()));
	say_map->setMapping(say_btn, say_txt);
	connect(say_map, SIGNAL(mapped(QWidget *)), this, SIGNAL(say_signal(QWidget *)));
	connect(this, SIGNAL(say_signal(QWidget *)), this, SLOT(Say(QWidget *)));

	QPushButton * mate_btn = new QPushButton("&Mate", options);
	connect(mate_btn, SIGNAL(clicked()), this, SLOT(Mate()));
	vbox->addWidget(mate_btn);


	vbox->addStretch();
}

void AgentInterface::Say(QWidget * txt) {
	QLineEdit * le = (QLineEdit *) txt;
	std::string msg = le->text().toStdString();
	WorldWindow * world = (WorldWindow*) parentWidget();
	send_message(agent->get_port(), world->get_port(), SOUND, msg.length(), msg.c_str());
}

void AgentInterface::Mate(void) {
	WorldWindow * world = (WorldWindow*) parentWidget();
	send_message(world->get_port(), agent->get_port(), MATE, 0);
}

