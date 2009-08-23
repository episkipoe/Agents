//add_reproduction
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <common/shmem.h>
#include <common/randomhelp.h>
#include <nnets/nnets.h>
int main(int argc, char *argv[]) {
	srandom(200);
	if(argc<2) return 0;
	Net myNet;
	//printf("add_reproduction started with file %s port %s\n", argv[0], argv[1]);
	char * genome_file = argv[0];
	int port = atoi(argv[1]);
	stringstream net_file;  net_file<<genome_file<<"."<<port<<".net";
	myNet.loadFromFile(net_file.str().c_str());
	int num_outputs=5;
	char output_name[] = "reproduction";
	for(int o = 0 ; o < num_outputs; o++) {
		myNet.addNeuron(OUTPUT, output_name);
	}
	myNet.connectSubnets("visual_cortex", output_name, -0.5, 1.25);
	myNet.saveToFile(net_file.str().c_str());
}

//end

