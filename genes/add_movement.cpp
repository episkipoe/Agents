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

	int num_inputs = 10;
	Neuron ** input_neurons = new Neuron*[num_inputs];
	int num_hidden = 5;
	Neuron ** hidden_neurons = new Neuron*[num_hidden];
	int num_outputs = 3;
	Neuron ** output_neurons = new Neuron*[num_outputs];
	int num_geneneurons = 2;
	Neuron ** gene_neurons = new Neuron*[num_geneneurons];


	char input_name[] = "eye";
	char hidden_name[] = "visual";
	char output_name[] = "move";
	char gene_name[] = "//move";
	stringstream net_file;  net_file<<genome_file<<"."<<port<<".net";
	myNet.loadFromFile(net_file.str().c_str());

	for(int h = 0 ; h < num_hidden; h++) {
		hidden_neurons[h] = myNet.addNeuron(HIDDEN, hidden_name);
	}

	for(int i = 0 ; i < num_inputs; i++) {
		input_neurons[i] = myNet.addNeuron(INPUT, input_name);

		for(int h = 0 ; h < num_hidden; h++) {
			float weight = randomFloat(-0.3,1.1);
			myNet.addSynapse(input_neurons[i], hidden_neurons[h], weight);
		}
	}

	for(int o = 0 ; o < num_outputs; o++) {
		output_neurons[o] = myNet.addNeuron(OUTPUT, output_name);

		for(int h = 0 ; h < num_hidden; h++) {
			float weight = randomFloat(-0.3,1.5);
			myNet.addSynapse(hidden_neurons[h], output_neurons[o], weight);
		}
	}
	for(int o = 0 ; o < num_geneneurons; o++) {
		gene_neurons[o] = myNet.addNeuron(OUTPUT, gene_name);

		for(int h = 0 ; h < num_hidden; h++) {
			float weight = randomFloat(-0.25,1.2);
			myNet.addSynapse(hidden_neurons[h], gene_neurons[o], weight);
		}
	}

	delete [] input_neurons;
	delete [] hidden_neurons;
	delete [] output_neurons;
	delete [] gene_neurons;
	myNet.saveToFile(net_file.str().c_str());
}

//end

