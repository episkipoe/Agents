//attack
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
	if(argc<2) return 1;
	int myPort = atoi(argv[0]);
	int target = atoi(argv[1]);
	send_message(myPort, 49152, KILL, sizeof(int), &target);
}

//end

