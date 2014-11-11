/*
**   NEURON
** Important function(s)
**  Neuron::fire
*/

#include "nnets.h"

//Constructor
Neuron::Neuron()
{
	name = "neuron";

	index = 0;
	type=HIDDEN;
	threshold = randomFloat(MIN_OUTPUT,0.6);
	prevBiasChange = 0;
	time_since_last_fired=1;

	reset();
}
Neuron::Neuron(int num, int theType)
{
	name = "neuron";

	index = num;
	type=theType;
	if(type==OUTPUT) {
		threshold = 0;
	} else {
		threshold = randomFloat(MIN_OUTPUT,0.6);
	}
	prevBiasChange = 0;
	time_since_last_fired=1;

	reset();
}

//Destructor
Neuron::~Neuron()
{
}

void Neuron::load(ifstream &is) {
	is>>where;
	name = loadString(is);
	is.read((char*)&index, sizeof(int));
	is.read((char*)&type, sizeof(int));
	is.read((char*)&threshold, sizeof(float));
	is.read((char*)&signal, sizeof(float));
	is.read((char*)&fireValue, sizeof(float));
	is.read((char*)&prevBiasChange, sizeof(float));
	is.read((char*)&time_since_last_fired, sizeof(int));
}

void Neuron::save(ofstream &os) {
	os<<where;
	saveString(os, name);
	os.write((char*)&index, sizeof(int));
	os.write((char*)&type, sizeof(int));
	os.write((char*)&threshold, sizeof(float));
	os.write((char*)&signal, sizeof(float));
	os.write((char*)&fireValue, sizeof(float));
	os.write((char*)&prevBiasChange, sizeof(float));
	os.write((char*)&time_since_last_fired, sizeof(int));
}

void Neuron::reset(void) {
	signal = 0;
	fireValue = 0;
}

void Neuron::setFireValue(void) {
	fireValue = 2 * (1 / (1 + expf(-signal))) - 1; /* from -1 to 1*/
	if(fireValue>threshold) { 
		signal=0.0;
		time_since_last_fired=0;
		//printf("idx %i type %i : setFireValue %g from signal %g threshold is %g\n", index, type, fireValue, signal, threshold);
	} else {
		time_since_last_fired++;
		fireValue=0.0; 
	}
}

float Neuron::getFireValue(void) {
	return fireValue;
}

void Neuron::draw(void) {
}


