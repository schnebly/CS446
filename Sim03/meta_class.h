/*
  Author: James Schnebly
  Title: meta_class.h
  Class: CS446/646
  Date: 9/11/2017 
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class MetaDataNode
{
public:
	MetaDataNode();
	MetaDataNode(char x, string strx, int intx);
	~MetaDataNode();


	//set and get functions
	void setHardwareChar(char x);
	void setInstruction(string x);
	void setCycles(int x);
	char getHardwareChar();
	string getInstruction();
	int getCycles();

	//test print function
	void printNode();


private:
	char hardwareChar;
	string instruction;
	int cycles;

};