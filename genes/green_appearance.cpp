//green_appearance
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <messages/message.h>
#include <common/randomhelp.h>
int main(int argc, char *argv[]) {
	printf("/newborn\n");
	int myPort = atoi(argv[0]);
	srandom(500);
	int width=16, height=16;
	int size=width*height*4;
	int header_size=sizeof(int)+sizeof(int);  int buffer_size=header_size+size;
	char * data;  data = new char[buffer_size]; char * loading_zone=data;
	memset(data, 0, buffer_size);
	memcpy(loading_zone, &width, sizeof(int));
	loading_zone+=sizeof(int);
	memcpy(loading_zone, &height, sizeof(int));
	loading_zone+=sizeof(int);
	for(int i=0;i<size;) {
		loading_zone[i++]=0;
		loading_zone[i++]=200;
		loading_zone[i++]=0;
		loading_zone[i++]=255;
	}
	//memcpy(loading_zone, data, size);

	send_message(myPort, 49152, VISION, buffer_size, data);
	delete [] data;
	return 0;
}

//end

