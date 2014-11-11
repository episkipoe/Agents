#ifndef GENOME_H
#define GENOME_H

#include <string>
#include <sstream>
#include <map>

using namespace std;

/**	Genome
	\brief manages a genome file and transcribes genes from it

	a genome file should have at minimum a //body block which is transcribed first, builds the organism and contains the event loop

*/
class Genome {
public:
	Genome();
	Genome(std::string f);

	/*fileIO*/
	int addLibrary(char * library);
	/** 
	attaches a genome file to the object
	@param f the filename of the genome 
	*/
	void attachFile(string f) { filename = f; }

	/*genes*/
	/**
	called by the world, transcribes the body and relays the assigned port
	@param port string representation of the organism's assigned port on which the world will communicate with the organism
	*/
	int buildOrganismFromGenome(const char * port=0);

	/**
	@param name the identifier of the gene in the genome file
	*/
	pid_t transcribeGene(string name, int n_args, ...);
	pid_t transcribeGene(string name, char ** args) ;
	pid_t transcribeGeneIfSubnetFires(map<string,int>&output_length, map<string,unsigned char*>&output_data, string subnet, float threshold, string gene, int n_args, ...) ;

private:
	string filename;
	stringstream libraries;

	/**
	//body indicates the gene program with the main event loop of the organism
	*/
	static bool isBodyCodon(char * line);
	/**
	//end indicates the stoping point of a gene within the genome
	*/ 
	static bool isEndCodon(char * line); 

	/**
	reads from current position to stop codon
	*/	
	static void readIntoStream(ifstream  &loadStream, stringstream &out_stream) ;

public: /*helpers for organism genes that manipulate genomes, such as reproduction*/ 
	static bool getGene(const char * filename, const char * gene_name, stringstream &out_gene_body, int offset=0);
};

#endif
