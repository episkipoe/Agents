#ifndef WORLD_WINDOW_H
#define WORLD_WINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "AgentInterface.h"

class WorldWindow: public QMainWindow {
Q_OBJECT

public:
	WorldWindow(QWidget * parent=0);
	~WorldWindow();

/*event dispatcher*/
	bool is_paused;

public slots:
	void agent_timer(void);

protected:
	void keyPressEvent(QKeyEvent * event);
	void keyReleaseEvent(QKeyEvent * event);

/*Networking*/
public: 
	void set_port(int port_no);
	int get_port() { return myPort; }

private:
	int myPort, sock;

/*Tools*/
private:
	void CreateMenu(void);
public slots:
	void LifeAndDeathOptions(void);
	void AddOrganism(void);
	void Nuke(void);

	void InteractionOptions(Agent * agent = 0);

	void AgentOptions(void);
	void ShowProperties(void);

	void DisplayOptions(void);
signals:
	void agent_signal(QWidget *);
};

#endif

