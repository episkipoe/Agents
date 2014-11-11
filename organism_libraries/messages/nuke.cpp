#include <string>
#include "message.h"

int main(int argc, char * argv[]) {
	int port = 49152;
	if(argc>1) port = atoi(argv[1]);
	send_message(port, port, KILL, sizeof(int), &port);
	system("killall");
}
