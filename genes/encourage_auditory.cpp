//encourage_auditory
#include <common/randomhelp.h>
#include <nnets/nnets.h>
#include <sstream>
int main(int argc, char *argv[]) {
	if(argc<2 || !success(50)) return 0;
	Net myNet;
	//printf("add_auditory started with file %s port %s\n", argv[0], argv[1]);
	char * genome_file = argv[0];
	int port = atoi(argv[1]);
	float amt=0.01;
	if(argc>2) amt = atof(argv[2]);
	stringstream net_file;  net_file<<genome_file<<"."<<port<<".net";
	myNet.loadFromFile(net_file.str().c_str());
	myNet.reward("ear", amt);
	myNet.reward("hearing", amt);
	myNet.reward("talk", amt);
	myNet.saveToFile(net_file.str().c_str());
}

//end

