#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

std::string loadString(std::ifstream & fileIO);
std::string randomName(void);

void saveString(std::ofstream & fileIO, std::string toSave);

float getPctFromCharArr(unsigned char * data, int data_length);
std::string getStringFromCharArr(unsigned char * data, int data_length);
#endif
