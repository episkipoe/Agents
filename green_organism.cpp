#include <stdlib.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <genome/genome.h>
#include <nnets/nnets.h>
#include <messages/message.h>
#include <common/shmem.h>
Net myNet;
Genome myGenome;
int myPort=49153;  
int worldPort=49152;
stringstream worldportstr; 
int n_children=0;
vector<int>children;
int organism_of_focus=worldPort;
stringstream portstr; 
char * genome_file;

int check_for_message(int sock) {
	static char * data=0;
	int message_length, sender;
	if(!data) data = new char[MAXMSGSIZE+1];
	MessageType type = get_message(sock, &data, message_length, &sender);
	if(type==NO_MESSAGE) return 0;
	//printf("organism recvd %i from %i\n", type, sender);
	if(type == SOUND) {
		char * msg = getStringFromData(data, message_length);
		//printf("%i) I hear sound %s length %i\n", myPort, msg, message_length);
		delete [] msg;
		if(sender!=myPort) { /*ignore sounds from myself*/
			myNet.feedInput(data, message_length, "ear");
			usleep(200);
			myNet.addClick();
			usleep(200);
			myGenome.transcribeGene("//encourage_auditory", 2, genome_file, portstr.str().c_str());
		}
	} else if(type == VISION) {
		//printf("start vision\n");
		if(message_length<=0) {
			organism_of_focus=worldPort;
			if(success(30)) {
				char data[2]={60,0};
				send_message(myPort, worldPort, TURN, 1, data);
				usleep(1600);
			}
		} else {
			memcpy(&organism_of_focus, data, sizeof(int));
			myNet.feedInput(data, message_length, "eye");
			char * dx_str = &data[sizeof(int)];
			float dx;
			memcpy(&dx, dx_str, sizeof(float));
			/*if(myPort==49153) {
				printf("%i at %g\n", organism_of_focus, dx);
			}*/
			if(dx<2.0) {
				if(std::find(children.begin(), children.end(), organism_of_focus) == children.end()) {
					stringstream destination_str;  destination_str<<organism_of_focus;
					myGenome.transcribeGene("//eat", 2, portstr.str().c_str(), destination_str.str().c_str());
				}
			}
			//printf("%i focus on %i dx %g\n", myPort, organism_of_focus, dx);
		}
		//printf("end vision\n");
	} else if(type == EAT) {
		int victim;
		memcpy(&victim, data, sizeof(int));
		if(victim==myPort) {
			/*printf("I'm being eaten\n");*/
		} else {
			myNet.reward("eat", 0.5);
		}
	} else if(type == KILL) {
		//printf("%i received kill message\n", myPort);
		exit(0);
	} else if(type == MATE) {
		if(sender == worldPort) { /*mate directive*/
			if(organism_of_focus==worldPort) { /*I can't see anybody, mate with self*/
				stringstream child_file;  child_file<<genome_file<<n_children;
				myGenome.transcribeGene("//asexual_reproduction", 4, portstr.str().c_str(), genome_file, child_file.str().c_str(), worldportstr.str().c_str()) ;
			} else { /*transmit request to potential mate*/
				printf("%i) send mate request to %i\n", myPort, organism_of_focus);
				send_message(myPort, organism_of_focus, MATE, strlen(genome_file), genome_file);
			}
		} else { /*the message came from a potential mate*/
			char * mate_genome = getStringFromData(data, message_length);
			printf("%i) received mate request from %i (%s)\n", myPort, sender, mate_genome);
			stringstream child_file;  child_file<<genome_file<<n_children;
			myGenome.transcribeGene("//sexual_reproduction", 5, portstr.str().c_str(), genome_file, mate_genome, child_file.str().c_str(), worldportstr.str().c_str()) ;
		}
	} else if(type == NEW_ORGANISM) {
		int child_id;
		memcpy(&child_id, data, sizeof(int));
		children.push_back(child_id);
		n_children++;
	} else if(type == DISPLAY) {
		myNet.printStats(1);
	} else if(type == SAVE) {
		char * filename = getStringFromData(data, message_length);
		stringstream cmd;
		cmd<<"cp "<<genome_file<<" "<<filename;
		system(cmd.str().c_str());
	}
	return 1;
}

int main (int argc, char * argv[]) {
	worldportstr << worldPort;
	if(argc<2) return 1;
	genome_file = argv[1];  if(!genome_file) return 1;
	myGenome.attachFile(genome_file);
	if(argc>2) myPort=atoi(argv[2]); 
	portstr << myPort;
	int sock = UDP_bind(myPort);
	//printf("my file is %s waiting on port %i\n", genome_file, myPort);

	myGenome.transcribeGene("//green_appearance", 1, portstr.str().c_str());

	srandom(100);
	stringstream net_file;  net_file<<genome_file<<"."<<myPort<<".net";
	myNet.name = net_file.str();
	myNet.saveToFile(net_file.str().c_str());
	myGenome.transcribeGene("//add_eye", 2, genome_file, portstr.str().c_str());
	myGenome.transcribeGene("//add_eye", 2, genome_file, portstr.str().c_str());
	myGenome.transcribeGene("//add_auditory", 2, genome_file, portstr.str().c_str());
	myGenome.transcribeGene("//add_reproduction", 2, genome_file, portstr.str().c_str());
	myGenome.transcribeGene("//add_movement", 2, genome_file, portstr.str().c_str());
	myGenome.transcribeGene("//add_turn", 2, genome_file, portstr.str().c_str());
	myGenome.transcribeGene("//connect_subnets", 2, genome_file, portstr.str().c_str());
	myNet.loadFromFile(net_file.str().c_str());
	myNet.printStats();

	//printf("body\n");

	stringstream child_file;  child_file<<genome_file<<n_children;
	//myGenome.transcribeGene("//asexual_reproduction", 4, portstr.str().c_str(), genome_file, child_file.str().c_str(), worldportstr.str().c_str());
	
	int n=4;
	while(1) {
		try {
			int status; while (waitpid(-1, &status, WNOHANG)>0);

			int nMessages=0; do {
				nMessages = check_for_message(sock);
			} while (nMessages>0);

			myNet.addClick();

			map<string,int>output_length; map<string,unsigned char*>output_data;
			myNet.getOutput(output_length, output_data);

			myGenome.transcribeGeneIfSubnetFires(output_length, output_data, "talk", 0.25, "//say", 1, portstr.str().c_str()) ;

			myGenome.transcribeGeneIfSubnetFires(output_length, output_data, "move", 0.2, "//move", 1, portstr.str().c_str()) ;

			myGenome.transcribeGeneIfSubnetFires(output_length, output_data, "turn", 0.1, "//turn", 1, portstr.str().c_str()) ;

			myGenome.transcribeGeneIfSubnetFires(output_length, output_data, "attack", 0.1, "//attack", 1, portstr.str().c_str()) ;

			myGenome.transcribeGeneIfSubnetFires(output_length, output_data, "eat", 0.2, "//eat", 1, portstr.str().c_str()) ;

			stringstream child_file;  child_file<<genome_file<<n_children;
			myGenome.transcribeGeneIfSubnetFires(output_length, output_data, "reproduction", 0.33, "//asexual_reproduction", 4, portstr.str().c_str(), genome_file, child_file.str().c_str(), worldportstr.str().c_str()) ;
	
			usleep(1000);
			//sleep(2);
		} 
		catch (...) {
			printf("organism %i error\n", myPort);
		}
	}
	close (sock);
	
	return 1;
}

//end

