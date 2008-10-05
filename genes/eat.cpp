//eat
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <messages/message.h>
int main(int argc, char *argv[]) {
	if(argc<2) return 1;
	int myPort = atoi(argv[0]);
	int target = atoi(argv[1]);
	printf("eat %i => %i\n", myPort, target);
	send_message(myPort, 49152, EAT, sizeof(int), &target);
}

//end

