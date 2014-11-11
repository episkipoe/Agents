#include "nnets.h"

void Net::feedInput(const char * data, int data_length, const char * data_type) {
	int nNeurons=0;
	list <Neuron*>::iterator curNode;
	for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
		if(!data_type || !strcmp(data_type, (*curNode)->name.c_str())) {
			nNeurons++;
		}
	}
	if(nNeurons<=0) return ;

	float * signal= new float[nNeurons];
	memset(signal, 0, sizeof(float)*nNeurons);
	int data_read = 0;
	while (data_read < data_length) {
		int nIdx = 0;
		for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
			if(!data_type || !strcmp(data_type, (*curNode)->name.c_str())) {
				if(data[data_read]<0) {
					signal[nIdx]+=(int)(-2*data[data_read]);
				} else {
					signal[nIdx]+=(int)data[data_read];
				}
				data_read++; nIdx++;
				if(data_read>=data_length) break;
			}
		}
	}
	int nIdx = 0;
	for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
		if(!data_type || !strcmp(data_type, (*curNode)->name.c_str())) {
			(*curNode)->signal += signal[nIdx++];
			//printf("%i => %g\n", (*curNode)->index, signal[nIdx-1]);
		}
	}

	delete [] signal;
}

void Net::addClick(void) {
	list <Neuron*>::iterator node_iterator;
	for(node_iterator = neuron.begin() ; node_iterator != neuron.end() ; node_iterator++){
		Neuron * curNode = *node_iterator;
		curNode->setFireValue();
	}

	list<Synapse>::iterator curSynapse;
	for (curSynapse = synapse.begin() ; curSynapse != synapse.end() ; curSynapse++) {
		if(curSynapse->fire_timer>0) {
			curSynapse->fire_timer--;
			continue;
		}
		curSynapse->fire_timer = curSynapse->fire_frequency; 
		float fireValue = curSynapse->from->getFireValue();
		if(!curSynapse->from->time_since_last_fired) {
			//printf("synapse transfer to %i type %i fv %g with weight %g\n", curSynapse->to->index, curSynapse->to->type, fireValue, curSynapse->weight);
		}
		curSynapse->to->signal += curSynapse->weight * fireValue;
		if(!curSynapse->from->time_since_last_fired && !curSynapse->to->time_since_last_fired) {
			curSynapse->weight+=curSynapse->potentiation;
		}
	}
	data.click++;
}

void Net::getOutput(map<string,int>&output_length, map<string,unsigned char*>&output_data) {
	list <Neuron*>::iterator node_iterator;
	for(node_iterator = neuron.begin() ; node_iterator != neuron.end() ; node_iterator++){
		Neuron * curNode = *node_iterator;
		output_length[curNode->name]++;
	}
	map<string,int>cur_pos;
	map<string,int>::iterator map_iterator;
	for( map_iterator = output_length.begin() ; map_iterator != output_length.end() ; map_iterator++) {
		string type = map_iterator->first;
		output_data[type] = new unsigned char[map_iterator->second];
		cur_pos[type]=0;
	}
	list<Neuron>outputs;
	for(node_iterator = neuron.begin() ; node_iterator != neuron.end() ; node_iterator++){
		Neuron * curNode = *node_iterator;
		if(curNode->type==OUTPUT) {
			unsigned char val = 255*curNode->fireValue;
			string type = curNode->name;
			output_data[type][cur_pos[type]] = val;
			cur_pos[type]++;
			//cout<<"output Node: "<<curNode->index<< " "<<type<<" fires with "<<curNode->fireValue<<endl;
		}
	}
}

bool Net::subnetFires(string subnet, double threshold_percent) {
	int n_nodes=0;  int signal=0;
	list <Neuron*>::iterator node_iterator;
	for(node_iterator = neuron.begin() ; node_iterator != neuron.end() ; node_iterator++){
		Neuron * curNode = *node_iterator;
		if(curNode->type!=OUTPUT) continue;
		if(subnet!=curNode->name) continue;
		unsigned char val = 255*curNode->fireValue;
		signal += val; 
		n_nodes++;
	}
	double max = 255.0 * n_nodes;
	return (signal > threshold_percent * max) ;
}

