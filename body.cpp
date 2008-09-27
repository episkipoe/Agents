//body
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <genome/genome.h>
#include <nnets/nnets.h>
#include <messages/message.h>
#include <common/shmem.h>
Net myNet;
Genome myGenome;
int myPort=49153;  
int worldPort=49152;
int n_children=0;
int organism_of_focus;

int check_for_message(int sock) {
	static char * data=0;
	int message_length, sender;
	if(!data) data = new char[MAXMSGSIZE+1];
	MessageType type = get_message(sock, &data, message_length, &sender);
	if(type==NO_MESSAGE) return 0;
	printf("organism recvd %i from %i\n", type, sender);
	if(type == SOUND) {
		char * msg = getStringFromData(data, message_length);
		printf("sound %s length %i\n", msg, message_length);
		delete [] msg;
		if(sender!=myPort) { /*ignore sounds from myself*/
			myNet.feedInput(data, message_length, "ear");
		}
	} else if(type == VISION) {
		myNet.feedInput(data, message_length, "eye");
	} else if(type == KILL) {
		printf("killed by message\n");
		exit(0);
	} else if(type == NEW_ORGANISM) {
		n_children++;
	}
	return 1;
}

int main (int argc, char * argv[]) {
	stringstream worldportstr; worldportstr << worldPort;
	if(argc<2) return 1;
	char * genome_file = argv[1];  if(!genome_file) return 1;
	myGenome.attachFile(genome_file);
	if(argc>2) myPort=atoi(argv[2]);
	stringstream portstr; portstr << myPort;
	int sock = UDP_bind(myPort);
	printf("my file is %s waiting on port %i\n", genome_file, myPort);

	while(1) {
		usleep(2000);
		sleep(2);
	}
	close (sock);
	
	return 1;
}

//end

