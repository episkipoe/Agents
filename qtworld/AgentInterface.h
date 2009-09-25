#ifndef AGENT_INTERFACE
#define AGENT_INTERFACE

#include <agent.h>
#include <QDockWidget>

class AgentInterface: public QDockWidget {
	Q_OBJECT
public:
	AgentInterface(Agent * a, QWidget * parent, Qt::WindowFlags flags = 0);

public slots:
	void Say(QWidget * txt);
	void Mate(void);

signals:
	void say_signal(QWidget *);

private:
	Agent * agent;
};

#endif
