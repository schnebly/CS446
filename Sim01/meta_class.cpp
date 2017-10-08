/*
  Author: James Schnebly
  Title: meta_class.cpp
  Class: CS446/646
  Date: 9/11/2017 
*/

#include <iostream>
#include <fstream>
#include <string>

#include "meta_class.h"

using namespace std;

MetaDataNode::MetaDataNode()
{
	//constructor
	hardwareChar = ' ';
	instruction = "";
	cycles = 0;
}

// parameterized constructor
MetaDataNode::MetaDataNode(char x, string strx, int intx)
{
	hardwareChar = x;
	instruction = strx;
	cycles = intx;
}

MetaDataNode::~MetaDataNode()
{
	//destructor
}

// getter and setter functions

void MetaDataNode::setHardwareChar(char x)
{
	hardwareChar = x;
}

void MetaDataNode::setInstruction(string x)
{
	instruction = x;
}

void MetaDataNode::setCycles(int x)
{
	cycles = x;
}

char MetaDataNode::getHardwareChar()
{
	return hardwareChar;
}

string MetaDataNode::getInstruction()
{
	return instruction;
}

int MetaDataNode::getCycles()
{
	return cycles;
}

void MetaDataNode::printNode()
{
	cout << endl;
	cout << hardwareChar << "(" << instruction << ")" << cycles;
	
}
