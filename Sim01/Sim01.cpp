/*
  Author: James Schnebly
  Title: Sim01.cpp
  Class: CS446/646
  Date: 9/11/2017 
*/

#include <iostream>
#include <fstream>
#include <string> 
#include <queue>

#include "config_class.cpp"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Error, you must supply a '.conf' file as an argument!" << endl;
		return 0;
	}

	string fileName;
	fileName = string(argv[1]);
	
	if(fileName.substr(fileName.find_last_of(".") + 1) != "conf")
	{
		cout << "Error, only a '.conf' file will be accepted as an argument!" << endl;
		return 0;
	}

	ifstream fin;
	Configuration config; 
	queue <string> metaDataQueue;
	string word = "";
	char parseChar = 'x';

	fin.clear();
	fin.open(argv[1]);

	config.loadConfig(fin);

	fin.close();

	fin.clear();
	fin.open(config.getMetaDataFile());

	
	while(parseChar != ':')
	{
		fin >> parseChar;
	}

	while(word != "End")
	{
		fin >> word;
		if(word != "End")
		{
			metaDataQueue.push(word.substr(0,word.length()-1));
		}
		
	}
	
	cout << endl;
	cout << "Meta-Data Metrics: " << endl;
	cout << "First Meta-Data: " << metaDataQueue.front() << endl;
	cout << "Last Meta-Data: " << metaDataQueue.back() << endl;

	fin.close();


	return 0;

}