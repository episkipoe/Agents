//say
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <messages/message.h>
int main(int argc, char *argv[]) {
	printf("/say %i %s %s\n", argc, argv[0], argv[argc-1]);
	int myPort = atoi(argv[0]);
	send_message(myPort, 49152, SOUND, strlen(argv[argc-1]), argv[argc-1]);
	return 0;
}

//end

