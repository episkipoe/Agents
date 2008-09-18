//move
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <messages/message.h>
int main(int argc, char *argv[]) {
	int myPort = atoi(argv[0]);
	char data[3]={0,0,0};
	printf("in move %i %s\n", argc, argv[1]);
	if(argc>1&&strlen(argv[1])>1) {
		printf("/move %i %i\n", argv[1][0], argv[1][1]);
		data[0]=argv[1][0];
		data[1]=argv[1][1];
	}
	send_message(myPort, 49152, MOVE, 2, data);
	return 0;
}

//end

