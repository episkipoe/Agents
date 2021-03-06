#include <string>
#include "message.h"

int main(int argc, char * argv[]) {
	if(argc<3) {
		printf("mate file port [to=49152] \n");
		return 0;
	}
	int srcPort=49153, destPort = 49152;
	if(argc>2) srcPort = atoi(argv[2]);
	if(argc>3) destPort = atoi(argv[3]);
	send_message(srcPort, destPort, NEW_ORGANISM, strlen(argv[1]), argv[1]);
	return 1;
}
