//add_eat
#include <string>
#include <stdio.h>
#include <stdlib.h>
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

	int num_attack_outputs = 3;
	int num_attack_geneneurons = 2;
	int num_eat_outputs = 3;
	int num_eat_geneneurons = 2;

	stringstream net_file;  net_file<<genome_file<<"."<<port<<".net";
	myNet.loadFromFile(net_file.str().c_str());

	/*
	for(int i = 0 ; i < 4; i++) {
		myNet.addNeuron(INPUT, "hunger");
	}
	myNet.connectSubnets("hunger", "eat", -0.1, 1.5);
	*/

	for(int o = 0 ; o < num_attack_outputs; o++) {
		myNet.addNeuron(OUTPUT, "attack");
	}
	for(int o = 0 ; o < num_attack_geneneurons; o++) {
		myNet.addNeuron(OUTPUT, "//attack");
	}
	for(int o = 0 ; o < num_eat_outputs; o++) {
		myNet.addNeuron(OUTPUT, "eat");
	}
	for(int o = 0 ; o < num_eat_geneneurons; o++) {
		myNet.addNeuron(OUTPUT, "//eat");
	}
	myNet.connectSubnets("visual_cortex", "attack", -0.25, 1.5);
	myNet.connectSubnets("visual_cortex", "//attack", -0.2, 1.2);
	myNet.connectSubnets("visual_cortex", "eat", -0.25, 1.5);
	myNet.connectSubnets("visual_cortex", "//eat", -0.2, 1.2);

	myNet.saveToFile(net_file.str().c_str());
}

//end

