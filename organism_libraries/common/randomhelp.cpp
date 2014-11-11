#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "randomhelp.h"
#include "macros.h"

/*
**  random integer between zero and max
*/
int random(int max)
{
	if(max<=0)
		return 0;
	return random() % max;
}

/*
**  returns a random little number (between 0.01 and something else)
**  used for initial weights and probably some other stuff
*/
float initWeight()
{
	return randomFloat(-1.0, 1.0);
/*
	float num;
	do {
		num = sin((double)random())/5;
	} while (fabs(num) < 0.001);
	return num;
*/
}
float randomFloat(float num1, float num2)
{
	float max = Max(num1, num2);
	float min = Min(num1, num2);
	float diff = max - min;
	double rpct = (double)(random()/(double)RAND_MAX);
	return (min + rpct*diff);
}

int numberBetween(int num1, int num2)
{
	if (num1 == num2)
		return num1;

	int max = Max(num1, num2);
	int min = Min(num1, num2);
	int diff = max - min + 1;
#if defined(_WIN32)
    return (rand() % diff) + min;
#else
	return (random() % diff) + min;
#endif
}

//skill is the percent likely of success
int success(int skill)
{
	//method one:  skill is the percent likely of success
	//this is used as a boolean
	int randNum = numberBetween(0, 100);
	if (skill > randNum)
		return 1;
	else
		return 0;
}
