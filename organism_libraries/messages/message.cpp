#include <string.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "message.h"
/*
 * send
 */

int send_message(int src_port, int dest_port, MessageType type) {
	return send_message(src_port, dest_port, type, 0);
}

int send_message(int src_port, int dest_port, MessageType type, ...) {
	va_list args;
	va_start (args, type);

	//printf("send type %i to port %i from %i\n", type, dest_port, src_port);
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock<0) return 0;

	struct sockaddr_in sa;
	sa.sin_family = PF_INET;
	sa.sin_addr.s_addr = htonl(0x7F000001);
	sa.sin_port = htons(dest_port);

	char * data=0; 
	int mtype_size, size;
	mtype_size = size = sizeof(MessageType);
	size+=sizeof(int);  /*src_port*/

	int payload_length = va_arg(args, int);
	char * payload = 0;
	if(payload_length>0) {
		payload = va_arg(args, char*);
		size+=payload_length;
	}

	data = new char[size];
	memcpy(data, &type, mtype_size);
	char * loading_zone = &data[mtype_size];
	memcpy(loading_zone, &src_port, sizeof(int));
	loading_zone+=sizeof(int);
	memcpy(loading_zone, payload, payload_length);

	va_end (args);
	int bytes_sent = sendto(sock, data, size, 0,(struct sockaddr*) &sa, sizeof(struct sockaddr_in));
	delete [] data;
	close(sock);

	return bytes_sent;
}

int send_data(int port, int size, void * data) {
	printf("send %i to %i\n", size, port);
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock<0) return 0;

	struct sockaddr_in sa;
	sa.sin_family = PF_INET;
	sa.sin_addr.s_addr = htonl(0x7F000001);
	sa.sin_port = htons(port);
 
	int bytes_sent = sendto(sock, data, size, 0,(struct sockaddr*) &sa, sizeof(struct sockaddr_in));
	close(sock);

	return bytes_sent;
}

/*
 * recv
 */

int UDP_bind(int port) {
	int sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	struct sockaddr_in sa;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(port);
	if (bind(sock,(struct sockaddr *)&sa, sizeof(struct sockaddr))<0) sock = -1;
	fcntl(sock, F_SETFL, O_NONBLOCK);
	return sock;
}

MessageType get_message(int sock, char ** data, int &message_length, int *sender) {
	char * d = *data;
	int header_length = (sizeof(MessageType)+sizeof(int));
	message_length = recvfrom(sock, d, MAXMSGSIZE, 0, NULL,  NULL);
	//if(message_length>0) { printf("recvd %i vs %i\n", message_length, header_length); }
	if(message_length<header_length) { return NO_MESSAGE; } 

	MessageType type;
	memcpy(&type, d, sizeof(MessageType));
	d+=sizeof(MessageType);

	int send_port;
	memcpy(&send_port, d, sizeof(int));
	d+=sizeof(int);
	if(sender) *sender = send_port;

	message_length-=header_length;
	*data = d;
	return type;
}

char * getStringFromData(char * in_data, int message_length) {
	char * out_data;
	out_data = new char[message_length+1];
	strncpy(out_data, in_data, message_length);
	out_data[message_length]=0;
	return out_data;
}


