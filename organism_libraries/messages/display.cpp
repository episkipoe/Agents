#include <string>
#include "message.h"

int main(int argc, char * argv[]) {
	int srcPort=49152, destPort = 49153;
	if(argc>1) destPort = atoi(argv[1]);
	MessageType msg = DISPLAY;
	send_message(srcPort, destPort, msg, 0);
	return 1;
}
