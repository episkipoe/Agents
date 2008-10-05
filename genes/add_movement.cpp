//add_movement
#include <string>
#include <stdio.h>
#include <sstream>
#include <common/shmem.h>
#include <common/randomhelp.h>
#include <nnets/nnets.h>
int main(int argc, char *argv[]) {
	srandom(100);
	if(argc<2) return 0;
	Net myNet;
	//printf("add_movement started with file %s port %s\n", argv[0], argv[1]);
	char * genome_file = argv[0];
	int port = atoi(argv[1]);

	int num_outputs = 3;
	Neuron ** output_neurons = new Neuron*[num_outputs];
	int num_geneneurons = 2;
	Neuron ** gene_neurons = new Neuron*[num_geneneurons];


	char output_name[] = "move";
	char gene_name[] = "//move";
	stringstream net_file;  net_file<<genome_file<<"."<<port<<".net";
	myNet.loadFromFile(net_file.str().c_str());

	for(int o = 0 ; o < num_outputs; o++) {
		output_neurons[o] = myNet.addNeuron(OUTPUT, output_name);
	}
	for(int o = 0 ; o < num_geneneurons; o++) {
		gene_neurons[o] = myNet.addNeuron(OUTPUT, gene_name);
	}
	myNet.connectSubnets("visual_cortex", "move", -0.25, 1.5);
	myNet.connectSubnets("visual_cortex", "//move", -0.2, 1.2);

	myNet.saveToFile(net_file.str().c_str());
}

//end

