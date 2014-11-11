#ifndef MESSAGE_H
#define MESSAGE_H
enum MessageType {NO_MESSAGE, DISPLAY, SOUND, VISION, NEW_ORGANISM, EAT, KILL, MATE, MOVE, TURN, SLEEP, SAVE};

#define MAXMSGSIZE 65536

/*send*/
int send_message(int src_port, int dest_port, MessageType type);
int send_message(int src_port, int dest_port, MessageType type, ...);
int send_data(int port, void * &data, int size);

/*recv*/
int UDP_bind(int port);
MessageType get_message(int sock, char ** data, int &message_length, int * sender=0);
char * getStringFromData(char * in_data, int message_length);

int get_string(char * data, char ** out_str);

#endif

