//sexual_reproduction
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <common/randomhelp.h>
#include <messages/message.h>
#define MAX_LINES_PER_BLOCK 2048
double mutation_rate = 0.001;
int read_block(FILE * f, char block[][LINE_MAX]) {
	int line_number=0;
	while(fgets(block[line_number], LINE_MAX, f) != NULL) {
		if(strlen(block[line_number])<=1) break;
		if(!strncmp(block[line_number], "//mutation_rate", 15)) {
			mutation_rate = atof(&block[line_number][16]);
		}
		line_number++;
		if(line_number>=MAX_LINES_PER_BLOCK) break;
	}
	return line_number;
}
void write_block(FILE * f, char block[][LINE_MAX], int n_lines) {
	for(int i=0;i<n_lines;i++) {
		fputs(block[i], f);
	}
	fputs("\n", f);
}
void write_block_backwards(FILE * f, char block[][LINE_MAX], int n_lines) {
	for(int i=n_lines-1;i>=0;i--) {
		fputs(block[i], f);
	}
	fputs("\n", f);
}
void mutate_block(char block[][LINE_MAX], int n_lines) {
	for(int i=0;i<n_lines;i++) {
		for(int j = 0 ; j < strlen(block[i]); j++) {
			if(isdigit(block[i][j])) {
				if(numberBetween(0,100)>80) {
					block[i][j]++;
					return;
				} else if(numberBetween(0,100)<20) {
					block[i][j]--;
					return;
				} 
			}
		}
	}
}

void delete_random_line(char block[][LINE_MAX], int n_lines) {
	int line = numberBetween(0,n_lines);
	block[line][0]='/';
	block[line][1]='/';
}
void copy_random_line(char block[][LINE_MAX], int n_lines) {
	int line = numberBetween(0,n_lines);
	int len = strlen(block[line]);
	memcpy(&block[line][len], block[line], len);
}
enum Mutation {NONE, AMPLIFICATION, LINE_DUPLICATION, BLOCK_DUPLICATION, LINE_DELETION, BLOCK_DELETION, POINT_MUTATION, TRANSLOCATION, INVERSION, MAX_MUTATION};
Mutation get_mutation(void) {
	if(randomFloat(0.0,1.0)<mutation_rate) {
		return Mutation(numberBetween(1,MAX_MUTATION-1));
	}
	return NONE;
}
int sexual_reproduction(char * mother_file, char * father_file, char * child_file ) {
	//printf("sexual reproduction %s and %s\n", mother_file, father_file);
	FILE * fmom = fopen(mother_file, "r");
	FILE * fdad = fopen(father_file, "r");
	FILE * fchild = fopen(child_file, "w");
	char block[MAX_LINES_PER_BLOCK][LINE_MAX];
	while(1) {
		if(feof(fmom)) {
			fclose(fmom);
			fclose(fdad);
			fclose(fchild);
			return 1;
		}
		Mutation mutation = get_mutation();
		int n_lines;
		/*read from both blocks to keep them in sync
		  the block read second is the one that's written
		*/
		if(randomFloat(0.0,1.0) >= 0.5) {
			n_lines = read_block(fdad, block);
			n_lines = read_block(fmom, block);
		} else {
			n_lines = read_block(fmom, block);
			n_lines = read_block(fdad, block);
		}
		if(mutation==BLOCK_DELETION) continue;
		if(mutation==INVERSION) {
			write_block_backwards(fchild, block, n_lines);
			continue;
		} 
		if(mutation==LINE_DELETION) {
			delete_random_line(block, n_lines);
		}
		if(mutation==LINE_DUPLICATION) {
			copy_random_line(block, n_lines);
		}
		if(mutation==POINT_MUTATION) {
			mutate_block(block, n_lines);
		}
		write_block(fchild, block, n_lines);
		if(mutation==BLOCK_DUPLICATION) {
			write_block(fchild, block, n_lines);
		}
	}
	return 1;
}
int main(int argc, char * argv[]) {
	if(argc<3) return 1;
	int srcPort = atoi(argv[0]);
	sexual_reproduction(argv[1], argv[2], argv[3]);
	int destPort = 49152;
	if(argc>4) destPort = atoi(argv[4]);
	if(argc>5) mutation_rate = atof(argv[5]);
	send_message(srcPort, destPort, NEW_ORGANISM, strlen(argv[2]), argv[2]);
}
//end

