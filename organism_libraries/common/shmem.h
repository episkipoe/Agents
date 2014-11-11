#ifndef SHMEM_H
#define SHMEM_H
void push_to_shared_memory(int key, char * data, int size) ;
int read_from_shared_memory(int key, char * data, int size) ;
int pop_from_shared_memory(int key, char * data, int size) ;
#endif
