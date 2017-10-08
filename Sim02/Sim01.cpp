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
#include "meta_class.cpp"

using namespace std;

void loadOneMetaData(ifstream& fin, queue <MetaDataNode>& metaDataQueue, char& sym);
void setupCursor(ifstream& fin);
void printMetaData(queue <MetaDataNode> metaDataQueue);
void printToLog(Configuration config, ofstream& fout);
void processMetaDataNode(Configuration config, queue <MetaDataNode>& metaDataQueue, string x, ofstream& fout);

int main(int argc, char* argv[])
{

// error check for conf argument
	if (argc != 2)
	{
		cout << "Error, you must supply a '.conf' file as an argument!" << endl;
		return 0;
	}

	string fileName;
	fileName = string(argv[1]);

// error check for correct extension
	if(fileName.substr(fileName.find_last_of(".") + 1) != "conf")
	{
		cout << "Error, only a '.conf' file will be accepted as an argument!" << endl;
		return 0;
	}

cout << 1 << endl;
	ifstream fin;
	ofstream fout;

	Configuration config; 
	MetaDataNode mdNode;
	queue <MetaDataNode> metaDataQueue;

	string word = "";
	char parseChar = 'x';

cout << 2 << endl;

// Begin Parse + load of config file
	fin.clear();
	fin.open(argv[1]);
	if (fin.peek() == std::ifstream::traits_type::eof())
	{
		cout << "Error: Empty Configuration File" << endl;
		return 0;
	}
	config.loadConfig(fin);
	fin.close();
// End Parse + load of config file

cout << 3 << endl;

// Begin Parse + load of Meta Data
	fin.clear();
	fin.open(config.getMetaDataFile());
	if (fin.peek() == std::ifstream::traits_type::eof())
	{
		cout << "Error: Empty Meta-Data File" << endl;
		return 0;
	}
	setupCursor(fin);
	while(parseChar != '.')
	{
		loadOneMetaData(fin, metaDataQueue, parseChar);
	}
	fin.close();
// End Parse + load of Meta Data



// Begin Display to Monitor/File/Both
	if (config.getLogMF() == "Monitor")
	{
		// Display config to monitor
		config.printConfig();
		cout << endl;

		//Display Meta Data Metrics to Monitor
		cout << "Meta-Data Metrics" << endl;
		while(metaDataQueue.empty() != 1)
		{
			processMetaDataNode(config, metaDataQueue, config.getLogMF(), fout);
		}
	}
	else if (config.getLogMF() == "File")
	{
		fout.clear();
		fout.open(config.getLogFileName());

		// Display to config.getLogFileName()
		printToLog(config, fout);

		fout << "Meta-Data Metrics" << endl;
		while(metaDataQueue.empty() != 1)
		{
			processMetaDataNode(config, metaDataQueue, config.getLogMF(), fout);
		}

		fout.close();
	}
	else if (config.getLogMF() == "Both")
	{
		fout.clear();
		fout.open(config.getLogFileName());

		// Display config to monitor
		config.printConfig();
		cout << endl;

		// Display to config.getLogFileName()
		printToLog(config, fout);
		
		//Display Meta Data Metrics to Monitor
		cout << "Meta-Data Metrics" << endl;
		fout << "Meta-Data Metrics" << endl;
		while(metaDataQueue.empty() != 1)
		{
			processMetaDataNode(config, metaDataQueue, config.getLogMF(), fout);
		}

		fout.close();
	}
// EndDisplay to Monitor/File/Both

	
	return 0;

}


// function that loads one meta-data node into queue
void loadOneMetaData(ifstream& fin, queue <MetaDataNode>& metaDataQueue, char& sym)
{

	char finalSym;
	string finalWord;
	int finalNum;

	char e = 'x';

	fin >> finalSym;

	while(sym != '(')
	{
		fin >> sym;
	}

	fin >> sym;
	while(sym != ')')
	{
		finalWord += sym;
		fin >> sym;
	}

	if (fin.peek() == ';')
	{
		cout << "Error: Missing cycle in .mdf file." << endl;
		exit(0);
	}

	fin >> finalNum;
	fin >> sym;

	MetaDataNode node(finalSym, finalWord, finalNum);
	metaDataQueue.push(node);
}

// function that brings cursor to intended position
void setupCursor(ifstream& fin)
{
	char sym;
	while(sym != ':')
	{
		fin >> sym;
	}
}

// test function to print meta-data
void printMetaData(queue <MetaDataNode> metaDataQueue)
{
	while(metaDataQueue.empty() != 1)
	{
		metaDataQueue.front().printNode();
		metaDataQueue.pop();
	}
}

// function that prints config specs to log
void printToLog(Configuration config, ofstream& fout)
{

	fout << endl;
	fout << "Configuration File Data: " << endl;
	fout << "Version: " << config.getVersion() << endl;
	fout << "Meta-Data File: " << config.getMetaDataFile() << endl;
	fout << "Processor speed = " << config.getProcessor_ct() << endl;
	fout << "Monitor speed = " << config.getMonitor_ct() << endl;
	fout << "Hard Drive speed = " << config.getHDrive_ct() << endl;
	fout << "Printer speed = " << config.getPrinter_ct() << endl;
	fout << "Keyboard speed = " << config.getKeyboard_ct() << endl;
	fout << "Memory speed = " << config.getMemory_ct() << endl;
	fout << "Mouse speed = " << config.getMouse_ct() << endl;
	fout << "Speaker speed = " << config.getSpeaker_ct() << endl;
	fout << "Log: Log to " << config.getLogMF() << endl;
	fout << "Log File: " << config.getLogFileName() << endl;
	fout << "System Memory: " << config.getSysMemory() << endl;
	fout << endl;
}

// function that computes the process time of one node
void processMetaDataNode(Configuration config, queue <MetaDataNode>& metaDataQueue, string x, ofstream& fout)
{
	char mdc = metaDataQueue.front().getHardwareChar();
	string mdd = metaDataQueue.front().getInstruction();
	int numCycles = metaDataQueue.front().getCycles();

	if (mdc != 'O' & mdc != 'P' & mdc != 'I' & mdc != 'M' & mdc != 'S' & mdc != 'A')
	{
		cout << endl;
		cout << "Error: Invalid Meta-Data code. Please check the supplied .mdf file." << endl;

		if (x == "File" | x == "Both")
		{
			fout << endl;
			fout << "Error: Invalid Meta-Data code. Please check the supplied .mdf file." << endl;
		}

		exit(0);
	}

	if (numCycles < 0)
	{
		cout << endl;
		cout << "Error: Invalid Meta-Data cycle. Please check the supplied .mdf file." << endl;

		if (x == "File" | x == "Both")
		{
			fout << endl;
			fout << "Error: Invalid Meta-Data cycle. Please check the supplied .mdf file." << endl;
		}

		exit(0);
	}

	int total = 0;
	int ct = 0;

		if(mdd == "allocate")
		{
			ct = config.getMemory_ct();
		}
		else if (mdd == "run")
		{
			ct = config.getProcessor_ct();
		} 
		else if (mdd == "harddrive")
		{
			ct = config.getHDrive_ct();
		} 
		else if (mdd == "end")
		{
			ct = 0;
		} 
		else if (mdd == "keyboard")
		{
			ct = config.getKeyboard_ct();
		} 
		else if (mdd == "printer")
		{
			ct = config.getPrinter_ct();
		} 
		else if (mdd == "monitor")
		{
			ct = config.getMonitor_ct();
		} 
		else if (mdd == "start")
		{
			ct = 0;
		} 
		else if (mdd == "mouse")
		{
			ct = config.getMonitor_ct();
		} 
		else if (mdd == "speaker")
		{
			ct = config.getSpeaker_ct();
		} 
		else if (mdd == "block")
		{
			ct = config.getMemory_ct();
		} 
		else
		{
			cout << endl;
			cout << "Error: Invalid Meta-Data Descriptor. Please check the supplied .mdf file." << endl;
			cout << endl;

			if (x == "File" | x == "Both")
			{
				fout << endl;
				fout << "Error: Invalid Meta-Data Descriptor. Please check the supplied .mdf file." << endl;
				fout << endl;
			}
		}

		total = ct * numCycles;

		metaDataQueue.pop();

		if(total != 0 & (x == "Monitor" | x == "Both"))
		{
			cout << mdc << "(" << mdd << ")" << numCycles << " - " <<
			total << " ms" << endl;

		}

		if (total != 0 & (x == "File" | x == "Both"))
		{
			fout << mdc << "(" << mdd << ")" << numCycles << " - " <<
			total << " ms" << endl;
		}
}
		
