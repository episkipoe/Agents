#ifndef AGENT_H
#define AGENT_H

#include <common/common.h>
#include <genome/genome.h>
#include <sstream>
#include <vector>

enum Status {OK, ASLEEP, EATEN, DEAD};

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
	Agent(Genome * g, pid_t mypid, int myport);
	~Agent();

	Attributes attr;
	std::string getName(void) { 
		stringstream s; s<<"Agent "; s<<port;
		return s.str();
	}

	/*graphics*/
	void set_appearance(char * data, int data_size);
	void get_appearance(int &width, int &height, char **data);
	Point * get_location (void) { return &attr.location; } 
	void draw(void);

	/*communication*/
	int get_port(void) { return port; }
	pid_t get_pid(void) { return pid; }
	int kill(int politely=1);
	int sleep(void);
	int wake(void);

	/*actions*/
	int have_child(char * filename);
	void attack (Agent * target) ;
	void move (int x, int y) ;
	void turn (int angle) ;

	/*time*/
	void end_turn(void) ;

	float sort_value;

private:
	Genome * genome;
	pid_t pid;
	int port;

	Appearance avatar;
};
typedef std::vector<Agent *>::iterator agent_iter;
#endif
