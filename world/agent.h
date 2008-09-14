#ifndef AGENT_H
#define AGENT_H

#include <common/common.h>
#include <genome/genome.h>

enum Status {OK, ASLEEP};

struct Attributes {
	Point location;
	Status status;
	int energy;
	int age;
	float max_speed;
} ;

struct Appearance {
	int width, height;
	char * data;
} ;

class Agent {
public: 
	Agent(Genome * g, int mypid, int myport);
	~Agent();

	Attributes attr;
	void setAppearance(char * data, int data_size);
	void getAppearance(int &width, int &height, char **data);

	/*communication*/
	int get_port(void) { return port; }
	int kill(int politely=1);
	int sleep(void);
	int wake(void);

	int haveChild(char * filename);
	void attack (Agent * target) ;
	void move (int x, int y) ;

	/*time*/
	void endTurn(void) ;

private:
	Genome * genome;
	int pid, port;

	Appearance avatar;
};

#endif
