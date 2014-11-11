#include "nnets.h"

//Constructor
Synapse::Synapse(Neuron * whoFrom, Neuron * whoTo, int i)
{
	from  = whoFrom;
	to = whoTo;
	index = i;
	potentiation=randomFloat(-0.25, 0.25);
	fire_frequency=numberBetween(1,5);
	fire_timer=0;
	weight = initWeight();
}

Synapse::Synapse()
{
	potentiation=randomFloat(-0.25, 0.25);
	fire_frequency=numberBetween(0,5);
	fire_timer=0;
	weight = initWeight();
}

