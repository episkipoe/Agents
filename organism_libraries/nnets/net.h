#ifndef NET_H
#define NET_H

#include "neuron.h"
#include "synapse.h"
#include <string>
#include <map>

#define HEURISTICS  40

struct NetData
{
	int id;

	float learningRate;
	float momentum;
	float averageValue;
	int heuristicValue[HEURISTICS];
	int click;
	int totalClicks;
};

/*
**  NET
*/

class Net {
public:
//FUNCTIONS

	Net();

//creation
	Neuron * addNeuron(int type);  
	Neuron * addNeuron(int type, std::string name);
	void addSynapse(Neuron * from, Neuron * to);
	void addSynapse(Neuron * from, Neuron * to, float weight);
	void connectSubnets (string from, string to, float min_weight, float max_weight);

//fileIO
	void save(ofstream & saveStream);
	void load(ifstream & loadStream);
	
	void saveToFile(const char * file_name);
	void loadFromFile(const char * file_name);
	
//graphics
	void drawNet();
	void drawSynapse(Synapse * theSynapse);
	void integrate(Net newNet);

//Operation
	void feedInput(const char * data, int data_length, const char * data_type);
	void addClick(void);
	void getOutput(map<string,int>&output_length, map<string,unsigned char*>&output_data) ;
	bool subnetFires(string subnet, double threshold_percent);
	void clear(void);
	void reset(void);

	/*learning*/
	void reward(string subnet, float amt);
	void punish(float amt);

//gets
	Neuron * getNeuron(int index);
	Neuron * getNeuron(int type, int N);
	int countNeurons(int type);
	void removeNeuron(Neuron * node);
	Synapse * getSynapse(Neuron * from, Neuron * to, int N);
	Synapse * getSynapse(Neuron * from, int N);
	Synapse * getSynapse(int index);
	void removeSynapse(int index);
	void transferLists(list<Neuron*> & newNeurons, list<Synapse> & newSynapses);
	void transferIndices(int & newNeurons, int & newSynapses);


//VARIABLES
	NetData data;
	std::string name;

	int indexBestOutput;

	void printStats(int verbosity=0); 
private:
//Initialization
	void createNode(int theNode, short type=0, float threshold =0);  //theNode = the index number
	void createConnections(int which);

//Operation
	int getIndex(int nodeNum, int type);

//VARIABLES
	int neuronIndex;
	int synapseIndex;

	list<Neuron*> neuron;
	list<Synapse>synapse;
};

#endif
