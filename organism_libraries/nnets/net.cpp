#include "nnets.h"

Net::Net()
{
	clear();
}


void Net::clear(void)
{
	name="NONE";
	synapse.clear();
	neuron.clear();
	neuronIndex = 1;
	synapseIndex=-1;
	data.click = 0;
	data.totalClicks = 0;
	data.averageValue=0;
	data.learningRate = 2;
	data.momentum = 0.1;

	for(int i = 0 ; i < HEURISTICS ; i++){
		data.heuristicValue[i] = 0;
	}

}

//this is called each click.  resets fireValue to zero
void Net::reset(void) {
	list <Neuron*>::iterator curNode;
	for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
		(*curNode)->reset();
	}
}

Neuron * Net::addNeuron(int type)  {
	Point where;
	Neuron * newNeuron = new Neuron(neuronIndex++, type);
	neuron.push_back(newNeuron);
	return newNeuron;
}
Neuron * Net::addNeuron(int type, string name)  {
	Point where;
	Neuron * newNeuron = addNeuron(type);
	newNeuron->name = name;
	return newNeuron;
}

void Net::addSynapse(Neuron * from, Neuron * to) {
	Synapse newSynapse(from, to, synapseIndex--);

	if(from && to){
		newSynapse.weight = initWeight();
	}
	synapse.push_back(newSynapse);
}
void Net::addSynapse(Neuron * from, Neuron * to, float weight) {
	Synapse newSynapse(from, to, synapseIndex--);
	newSynapse.weight = weight;
	synapse.push_back(newSynapse);
}

void Net::connectSubnets (string from, string to, float min_weight, float max_weight) {
	list <Neuron*>::iterator fromNode, toNode;
	for(fromNode = neuron.begin() ; fromNode != neuron.end() ; fromNode++){
		if((*fromNode)->name != from) continue;
		for(toNode = neuron.begin() ; toNode != neuron.end() ; toNode++){
			if((*toNode)->name != to) continue;
	
			float weight = randomFloat(min_weight, max_weight);
			addSynapse((*fromNode), (*toNode), weight);
		}
	}
}

void Net::transferIndices(int & newNeurons, int & newSynapses) {
	newNeurons = neuronIndex;
	newSynapses = synapseIndex;
}
void Net::transferLists(list<Neuron*> & newNeurons, list<Synapse> & newSynapses) {
	newNeurons = neuron;
	newSynapses = synapse;
}

void Net::integrate(Net newNet) {
	clear();
	data = newNet.data;
	name = newNet.name;
	newNet.transferLists(neuron, synapse);
	newNet.transferIndices(neuronIndex, synapseIndex);
}

Neuron * Net::getNeuron(int index) {
	list <Neuron*>::iterator curNode;
	for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
		if(index==(*curNode)->index)
			return *curNode;
	}
	return NULL;
}


Neuron * Net::getNeuron(int type, int N) {
	list <Neuron*>::iterator curNode;
	for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
		if((*curNode)->isOfType(type)){
			if(N<=1)
				return *curNode;
			N--;
		}
	}
	return NULL;
}

int Net::countNeurons(int type) {
	int sum=0;
	list <Neuron*>::iterator curNode;
	for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
		if((*curNode)->isOfType(type)) sum++;
	}
	return sum;
}

void Net::removeNeuron(Neuron * node) {
	list<Synapse>::iterator curSynapse;
	for(curSynapse = synapse.begin() ; curSynapse != synapse.end() ; ){
		if(curSynapse->from == node|| curSynapse->to == node) {
			curSynapse = synapse.erase(curSynapse);
		} else {
			curSynapse++;
		}
	}

	list <Neuron*>::iterator curNode;
	for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
		if(node==(*curNode)) {
			neuron.erase(curNode);
			return;
		}
	}
}

Synapse * Net::getSynapse(Neuron * from, Neuron * to, int N) {
	list<Synapse>::iterator curSynapse;
	for(curSynapse = synapse.begin() ; curSynapse != synapse.end() ; curSynapse++){
		if(curSynapse->from == from && curSynapse->to == to) {
			if(N<=1)
				return &*curSynapse;
			N--;
		}
	}
	return NULL;
}

Synapse * Net::getSynapse(Neuron * from, int N) {
	list<Synapse>::iterator curSynapse;
	for(curSynapse = synapse.begin() ; curSynapse != synapse.end() ; curSynapse++){
		if(curSynapse->from == from){
			if(N<=1)
				return &*curSynapse;
			N--;
		}
	}
	return NULL;
}

Synapse * Net::getSynapse(int index) {
	list <Synapse>::iterator curSynapse;
	for(curSynapse= synapse.begin() ; curSynapse!= synapse.end() ; curSynapse++){
		if(index==curSynapse->index)
			return &*curSynapse;
	}
	return NULL;
}

void Net::removeSynapse(int index) {
	list <Synapse>::iterator curSynapse;
	for(curSynapse= synapse.begin() ; curSynapse!= synapse.end() ; curSynapse++){
		if(index==curSynapse->index) {
			synapse.erase(curSynapse);
			return;
		}
	}
}

void Net::printStats(int verbosity) {
	if(verbosity >= 0) {
		printf("%s %i neurons %i synapses\n", name.c_str(), neuron.size(), synapse.size());
	}

	if(verbosity >= 1) {
		list <Neuron*>::iterator curNode;
		for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
			printf("neuron %i ", (*curNode)->index);
			switch ((*curNode)->type) {
				case INPUT:
					printf("INPUT ");
					break;
				case HIDDEN:
					printf("HIDDEN ");
					break;
				case OUTPUT:
					printf("OUTPUT ");
					break;
			}
			printf(" %s\n", (*curNode)->name.c_str());
			printf("signal %g fileValue %g threshold %g\n", (*curNode)->signal, (*curNode)->fireValue, (*curNode)->threshold);
		}

		list<Synapse>::iterator curSynapse;
		for(curSynapse = synapse.begin() ; curSynapse != synapse.end() ; curSynapse++){
			printf("synapse %i from %i to %i  weight %g\n", curSynapse->index, 
				curSynapse->from->index, curSynapse->to->index, curSynapse->weight);
		}
	}
}

void Net::drawNet() {
	list <Neuron*>::iterator curNode;
	for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
		(*curNode)->draw();
	}

	list<Synapse>::iterator curSynapse;
	for(curSynapse = synapse.begin() ; curSynapse != synapse.end() ; curSynapse++){
		drawSynapse(&*curSynapse);
	}
}

void Net::save(ofstream & saveStream) {
	saveString(saveStream, name);
	saveStream.write((char*)&data, sizeof(NetData));

	//save private data
	saveStream.write((char*)(&neuronIndex), sizeof(int));
	saveStream.write((char*)(&synapseIndex), sizeof(int));

	//save neurons
	int count=0;
	list <Neuron*>::iterator curNode;
	for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
		count++;
	}
	saveStream.write((char*)(&count), sizeof(int));
	for(curNode = neuron.begin() ; curNode != neuron.end() ; curNode++){
		(*curNode)->save(saveStream);
	}

	//save synapses
	count=0;
	list <Synapse>::iterator curSynapse;
	for(curSynapse= synapse.begin() ; curSynapse!= synapse.end() ; curSynapse++){
		count++;
	}
	saveStream.write((char*)(&count), sizeof(int));
	for(curSynapse= synapse.begin() ; curSynapse!= synapse.end() ; curSynapse++){
		//cout<<"saving synapse #: "<<curSynapse->index<<endl;
		saveStream.write((char*)(&curSynapse->index), sizeof(int));
		saveStream.write((char*)(&curSynapse->weight), sizeof(float));
		saveStream.write((char*)(&curSynapse->potentiation), sizeof(float));
		saveStream.write((char*)(&curSynapse->fire_frequency), sizeof(char));
		saveStream.write((char*)(&curSynapse->fire_timer), sizeof(char));
		saveStream.write((char*)(&curSynapse->from->index), sizeof(int));
		saveStream.write((char*)(&curSynapse->to->index), sizeof(int));
	}
}

void Net::load(ifstream & loadStream) {
	clear();

	name = loadString(loadStream);
	loadStream.read((char*)&data, sizeof(NetData));

	//load private data
	loadStream.read((char*)(&neuronIndex), sizeof(int));
	loadStream.read((char*)(&synapseIndex), sizeof(int));

	//load neurons
	int count;
	loadStream.read((char*)(&count), sizeof(int));
	while(count>0){
		Neuron * newNode = new Neuron;
		newNode->load(loadStream);
		neuron.push_back(newNode);
		count--;
	}

	//load synapses
	loadStream.read((char*)(&count), sizeof(int));
	while(count>0){
		Synapse newSynapse;
		loadStream.read((char*)(&newSynapse.index), sizeof(int));
		loadStream.read((char*)(&newSynapse.weight), sizeof(float));
		loadStream.read((char*)(&newSynapse.potentiation), sizeof(float));
		loadStream.read((char*)(&newSynapse.fire_frequency), sizeof(char));
		loadStream.read((char*)(&newSynapse.fire_timer), sizeof(char));

		int idx;
		loadStream.read((char*)(&idx), sizeof(int));
		newSynapse.from = getNeuron(idx);
		loadStream.read((char*)(&idx), sizeof(int));
		newSynapse.to = getNeuron(idx);

		//cout<<"loading synapse #"<<newSynapse.index<<endl;
		synapse.push_back(newSynapse);
		count--;
	}

}

void Net::saveToFile(const char * file_name) {
	//printf("save to %s\n", file_name);
	ofstream saveStream(file_name, ios::out | ios::binary);
	save(saveStream);
}
void Net::loadFromFile (const char * file_name) {
	//printf("load from %s\n", file_name);
	ifstream loadStream(file_name, ios::in | ios::binary);
	load(loadStream);
}


void Net::drawSynapse(Synapse * theSynapse) {
	if(!theSynapse){
		cout<<"net.cpp:  drawSynapse:  asking me to draw a NULL synapse"<<endl;
		return;
	}
	if(!theSynapse->from || !theSynapse->to) return;
	if(theSynapse->from == theSynapse->to) return;

	Point begin = theSynapse->to->where;
	Point end = theSynapse->from->where;

//	drawArrow(begin, end, theSynapse->index, theSynapse->value);
}

