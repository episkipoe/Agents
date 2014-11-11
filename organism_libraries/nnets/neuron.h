#ifndef NEURON_H
#define NEURON_H
#include <common/point.h>
/*
**  NEURONS
*/

class Neuron {
	friend class Net;
public:
	Neuron();
	Neuron(int num, int theType);
	~Neuron();
	Neuron * operator=(Neuron * other) { return other; }
	void load(ifstream &os);
	void save(ofstream &is);

	Point where;
	string name;  //the public name of the node

//Operation
	void reset();
	void setFireValue(void);
	float getFireValue(void);

	bool isOfType(int ofType) { return (ofType == type); }
	void draw(void);

private:
	int index;  //the internal identifier of the node.
	int type;   //input/hidden/output

	float threshold, signal, fireValue; 

	float prevBiasChange;   //(momentum)
	int time_since_last_fired;
};

#endif
