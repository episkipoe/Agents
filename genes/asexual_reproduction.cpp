//asexual_reproduction
#include <string>
#include <limits.h>
#include <stdio.h>
#include <common/randomhelp.h>
#include <messages/message.h>
#define MAX_LINES_PER_BLOCK 2048
int read_block(FILE * f, char block[][LINE_MAX]) {
	int line_number=0;
	while(fgets(block[line_number], LINE_MAX, f) != NULL) {
		if(strlen(block[line_number])<=1) break;
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

int delete_random_line(block, n_lines) {
	int line = numberBetween(0,n_lines);
	block[line][0]='/';
	block[line][1]='/';
}

enum Mutation {NONE, AMPLIFICATION, DUPLICATION, LINE_DELETION, BLOCK_DELETION, POINT_MUTATION, TRANSLOCATION, INVERSION, MAX_MUTATION};
Mutation get_mutation(double mutation_rate) {
	if(randomFloat(0.0,1.0)<mutation_rate) {
		return Mutation(numberBetween(1,MAX_MUTATION-1));
	}
	return NONE;
}
int asexual_reproduction(char * mother_file, char * child_file, double mutation_rate ) {
	FILE * fmom = fopen(mother_file, "r");
	FILE * fchild = fopen(child_file, "w");
	char block[MAX_LINES_PER_BLOCK][LINE_MAX];
	while(1) {
		if(feof(fmom)) {
			fclose(fmom);
			fclose(fchild);
			return 1;
		}
		Mutation mutation = get_mutation(mutation_rate);
		int n_lines = read_block(fmom, block);
		if(mutation==BLOCK_DELETION) continue;
		if(mutation==INVERSION) {
			write_block_backwards(fchild, block, n_lines);
			continue;
		} 
		if(mutation==LINE_DELETION) {
			delete_random_line(block, n_lines);
		}
		if(mutation==POINT_MUTATION) {
			mutate_block(block, n_lines);
		}
		write_block(fchild, block, n_lines);
		if(mutation==DUPLICATION) {
			write_block(fchild, block, n_lines);
		}
	}
	return 1;
}
int main(int argc, char * argv[]) {
	if(argc<3) return 1;
	int srcPort = atoi(argv[0]);
	asexual_reproduction(argv[1], argv[2], 1e-4);
	int destPort = 49152;
	if(argc>3) destPort = atoi(argv[3]);
	send_message(srcPort, destPort, NEW_ORGANISM, strlen(argv[2]), argv[2]);
}
//end

