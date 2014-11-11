#include <string>
#include "message.h"

int main(int argc, char * argv[]) {
	if(argc<3) {
		printf("save file [port=49153]\n");
		return 0;
	}
	int srcPort=49152, destPort = 49153;
	if(argc>2) destPort = atoi(argv[2]);
	if(argc>3) srcPort = atoi(argv[3]);
	send_message(srcPort, destPort, SAVE, strlen(argv[1]), argv[1]);
	return 1;
}
