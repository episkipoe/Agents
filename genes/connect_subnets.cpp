//connect_subnets
#include <string>
#include <stdio.h>
#include <sstream>
#include <common/randomhelp.h>
#include <nnets/nnets.h>
int main(int argc, char *argv[]) {
	srandom(200);
	if(argc<2) return 0;
	Net myNet;
	//printf("add_auditory started with file %s port %s\n", argv[0], argv[1]);
	char * genome_file = argv[0];
	int port = atoi(argv[1]);
	stringstream net_file;  net_file<<genome_file<<"."<<port<<".net";
	myNet.loadFromFile(net_file.str().c_str());

	int max = myNet.countNeurons(HIDDEN);
	for(int i = 0 ; i < 60 ; i++) {
		Neuron * from = myNet.getNeuron(HIDDEN, numberBetween(0,max));
		Neuron * to = myNet.getNeuron(HIDDEN, numberBetween(0,max));
		float weight = randomFloat(-0.5,1.5);
		myNet.addSynapse(from, to, weight);
	}

	myNet.saveToFile(net_file.str().c_str());
}

//end

