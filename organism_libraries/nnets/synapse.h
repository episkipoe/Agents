#ifndef SYNAPSE_H
#define SYNAPSE_H
#include "neuron.h"
/*
**  SYNAPSES
*/

class Synapse {
	friend class Net;


public:
	Synapse(Neuron * whoFrom, Neuron * whoTo, int i);
	Synapse();

	void drawSynapse(void);

private:
	Neuron * from, * to;

	int index;
	float potentiation;  
	unsigned char fire_frequency, fire_timer;
	float weight;  
};

#endif
