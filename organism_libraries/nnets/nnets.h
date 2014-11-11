#ifndef NNETS_H
#define NNETS_H
using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include <common/common.h>

/*neuron types*/
#define INPUT   0
#define HIDDEN  1
#define OUTPUT  2

#define MIN_OUTPUT  0.0000453979   //expf on -10
#define MAX_OUTPUT  0.999955       //expf on  10

#include "net.h"

#endif
