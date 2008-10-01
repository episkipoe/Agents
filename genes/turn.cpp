//turn
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <messages/message.h>
int main(int argc, char *argv[]) {
	int myPort = atoi(argv[0]);
	char data[2]={0,0};
	if(argc>1&&strlen(argv[1])>0) {
		data[0]=argv[1][0];
	}
	send_message(myPort, 49152, TURN, 1, data);
	return 0;
}

//end

