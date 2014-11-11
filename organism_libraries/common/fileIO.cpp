#include <string.h>
#include <iostream>
#include "fileIO.h"

using namespace std;

//******************************LOAD*****************************
string loadString(ifstream & fileIO)
{
	//recover the length
 	int length = 0;
 	fileIO.read((char*)(&length), sizeof(int));
	if(length<1 || length>100){
		cout<<"fileIO.cpp: loadString: invalid string length:  "<<length<<endl;
	}
	char buff[length];
 	fileIO.read(buff, length);
	buff[length-1]=0; //ensure null terminated
 	string theString = string(buff);
	//cout<<"loading "<<buff<<endl;

	/*
	if (theString.length() == 0) {
 		return "";
	}
	*/

 	return theString;
}

//******************************SAVE*****************************
void saveString(ofstream & fileIO, string toSave)
{
	//save the size of the string  (needed for recovery)
	int length = toSave.length()+1;
	char buff[length];
	buff[length-1]=0;
	fileIO.write((char*)(&length), sizeof(int));
	//xfer to char
	for(int i = 0 ; i < length-1 ; i++) {
		buff[i] = toSave[i];
	}
	//save the string
	for(int i = 0 ; i < length ; i++) {

		fileIO.write(&buff[i], 1);
  		//cout<<"writing: "<<i<<": "<<buff[i]<<endl;
	}

}

float getPctFromCharArr(unsigned char * data, int data_length)
{
	int signal=0, max_signal = 255*data_length; 
	for(int i = 0; i<data_length; i++) {
		signal+=data[i];	
	}
	return (signal/(float)max_signal);
}

std::string getStringFromCharArr(unsigned char * data, int data_length)
{
	char * temp_data = new char[data_length+1];
	memcpy(temp_data, data, data_length);
	temp_data[data_length]=0;	
	string return_val = string(temp_data);
	delete [] temp_data;
	return return_val;
}

