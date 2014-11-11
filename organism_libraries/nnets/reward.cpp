#include "nnets.h"

void Net::reward(string subnet, float amt) {
	//printf("reward %s, %g\n", subnet.c_str(), amt);
	list<Synapse>::iterator curSynapse;
	for (curSynapse = synapse.begin() ; curSynapse != synapse.end() ; curSynapse++) {
		Neuron * from = curSynapse->from;
		Neuron * to = curSynapse->to;
		if(from->name == subnet) {
			if (from->time_since_last_fired <= curSynapse->fire_frequency) {
				curSynapse->weight*=1.05;
			} else {
				curSynapse->weight*=0.95;
			}
		} 
		if(to->name == subnet) {
			if (from->time_since_last_fired <= curSynapse->fire_frequency) {
				curSynapse->weight*=1.05;
			} else {
				curSynapse->weight*=0.95;
			}
		}
	}

}

