#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <string>

void push_to_shared_memory(int key, char * data, int size) {
	printf("push %i := %i%i\n", size, data[0], data[1]);
	int shmid = shmget(key, size, IPC_CREAT | 0666);
	if(shmid<0) return ;
	char * shm;
	if((shm = (char*)shmat(shmid, NULL, 0)) == (char *) -1) return ;
	memcpy(shm, data, size);
}

int read_from_shared_memory(int key, char * data, int size) {
	int shmid = shmget(key, size, 0666);
	if(shmid<0) return 0;
	char * shm;
	if((shm = (char*)shmat(shmid, NULL, 0)) == (char *) -1) return 0;
	memcpy(data, shm, size);
	printf("read %i := %i%i\n", size, data[0], data[1]);
	return 1;
}

int pop_from_shared_memory(int key, char * data, int size) {
	int shmid = shmget(key, size, 0666);
	if(shmid<0) return 0;
	char * shm;
	if((shm = (char*)shmat(shmid, NULL, 0)) == (char *) -1) return 0;
	memcpy(data, shm, size);
	shmdt(shm);
	shmctl(shmid, IPC_RMID, 0);
	return 1;
}

#if 0
int main() {
	char push_data[] = "test_shared";
	push_to_shared_memory(123, push_data, sizeof(push_data));

	char pop_data[64];
	pop_from_shared_memory(123, pop_data, sizeof(push_data));
	printf("revcd %s\n", pop_data);

	int val = 7;
	push_to_shared_memory(123, (char*)&val, sizeof(int));
	val = 49;
	pop_from_shared_memory(123, (char*)&val, sizeof(int));
	printf("val is %i\n", val);
	return 0;
}
#endif
