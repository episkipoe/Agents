#include <stdlib.h>
#include "execute.h"
#include "genome.h"
#include <common/fileIO.h>


using namespace std;

Genome::Genome() {
}

Genome::Genome(string f):filename(f) {
}

bool Genome::isBodyCodon(char * line) {
	int len = strlen(line);
	if(len<3) return false;
	if(len>6) len = 6;
	if(!strncmp(line, "//body", len)) return true; 
	return false;
}

bool Genome::isEndCodon(char * line) {
	int len = strlen(line);
	if(len<3) return false;
	if(len>5) len = 5;
	if(!strncmp(line, "//end", len)) return true;
	return false;
}

void Genome::readIntoStream(ifstream  &loadStream, stringstream &out_stream) {
	char line [LINE_MAX];
	while(loadStream.getline(line, LINE_MAX) != NULL) {
		if(isEndCodon(line)) {
			return;	
		}
		out_stream << line <<endl;
	}
}

bool Genome::getGene(const char * filename, const char * gene_name, stringstream &out_gene_body, int offset) {
	ifstream loadStream(filename, ios::in);

	char line [LINE_MAX];
	while(loadStream.getline(line, LINE_MAX) != NULL) {
		if(loadStream.eof()) break;
		if(!strncmp(line, gene_name, strlen(gene_name))) {
			if(--offset <= 0) {	
				readIntoStream(loadStream, out_gene_body);
				return true;
			}
		}
	}
	return false;
}

pid_t Genome::buildOrganismFromGenome(const char * port) {
	ifstream loadStream(filename.c_str(), ios::in);

	stringstream organism_body;

	char line [LINE_MAX];
	while(loadStream.getline(line, LINE_MAX) != NULL) {
		if(isBodyCodon(line)) {
			readIntoStream(loadStream, organism_body);
			return compile_and_execute(organism_body.str().c_str(), libraries.str().c_str(), 3, "pid", filename.c_str(), port);	
		} 
	}
	return -1;
}

pid_t Genome::transcribeGene(string name, int n_args, ...) {
	va_list args;
	va_start (args, n_args);

	ifstream loadStream(filename.c_str(), ios::in);

	stringstream gene_body;

	char line [LINE_MAX];
	while(loadStream.getline(line, LINE_MAX) != NULL) {
		if(loadStream.eof()) break;
		if(!strncmp(line, name.c_str(), name.length())) {
			readIntoStream(loadStream, gene_body);
			pid_t child = compile_and_execute(gene_body.str().c_str(), libraries.str().c_str(), n_args, args);
			if(child>=0) return child;
			gene_body.clear();
		}
	}
	return -1;
}


pid_t Genome::transcribeGene(string name, char ** args) {
	ifstream loadStream(filename.c_str(), ios::in);

	stringstream gene_body;

	char line [LINE_MAX];
	while(loadStream.getline(line, LINE_MAX) != NULL) {
		if(loadStream.eof()) break;
		if(!strncmp(line, name.c_str(), name.length())) {
			readIntoStream(loadStream, gene_body);
			pid_t child = compile_and_execute(gene_body.str().c_str(), libraries.str().c_str(), args);
			if(child >=0) return child;
			gene_body.clear();
		}
	}
	return -1;
}

pid_t Genome::transcribeGeneIfSubnetFires(map<string,int>&output_length, map<string,unsigned char*>&output_data, string subnet, float threshold, string gene, int n_args, ...) {
	map<string,int>::iterator map_iterator;
	map_iterator = output_length.find(subnet);
	if(map_iterator != output_length.end()) {
		float pct = getPctFromCharArr(output_data[subnet], output_length[subnet]);
		if(pct>threshold) {
			if(output_length.find(gene)!=output_length.end()) {
				va_list args;
				va_start (args, n_args);

				char ** argarr =0;
				argarr = (char**)calloc(sizeof(char*),(n_args+2));
				for(int i = 0; i < n_args ; i++) {
					argarr[i]=va_arg(args, char*);
				}
				va_end(args);

				string data = getStringFromCharArr(output_data[gene], output_length[gene]);
				argarr[n_args]=(char*)data.c_str();
				return transcribeGene(gene, argarr);
			}
		}
	}
	return 0;
}

