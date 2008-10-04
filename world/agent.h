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
	float heading, view_angle, view_distance;
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
	void set_appearance(char * data, int data_size);
	void get_appearance(int &width, int &height, char **data);
	Point * get_location (void) { return &attr.location; } 
	void draw(void);

	/*communication*/
	int get_port(void) { return port; }
	int kill(int politely=1);
	int sleep(void);
	int wake(void);

	int have_child(char * filename);
	void attack (Agent * target) ;
	void move (int x, int y) ;
	void turn (int angle) ;

	/*time*/
	void end_turn(void) ;

	float sort_value;

private:
	Genome * genome;
	int pid, port;

	Appearance avatar;
};

#endif
