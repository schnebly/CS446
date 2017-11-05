/*
  Author: James Schnebly
  Title: config_class.cpp
  Class: CS446/646
  Date: 9/11/2017 
*/


#include "config_class.h"

// constructor
Configuration::Configuration()
{
	version = 0.0;

	processor_ct = 0;
	monitor_ct = 0;
	hDrive_ct = 0;
	printer_ct = 0;
	keyboard_ct = 0;
	memory_ct = 0;
	mouse_ct = 0;
	speaker_ct = 0;

	sysMemory = 0;
	blockSize = 0;

	HDCount = 0;
	printerCount = 0;

	quantumNum = 0;
	cpuCode = "";

	metaDataFile = "";
	logMF = "";
	logFileName = "";
}

// destructor
Configuration::~Configuration()
{
	
}

// function that parses config file
void Configuration::loadConfig(ifstream& fin)
{
	string phrase = "";
	char sym;
	string configVarArray[] = {"Processor", "Monitor", "Hard", "Printer", "Keyboard", "Memory",
								"Mouse", "System", "Speaker", "size", "Scheduling"};
	fin >> sym;


	while (sym != ':')
	{
		fin >> sym;
	}
	fin >> version;
	sym = 'x';

	while (sym != ':')
	{
		fin >> sym;
	}
	fin >> metaDataFile;
	sym = 'x';

	while(phrase != "Log:")
	{
		fin >> phrase;
		if (phrase == configVarArray[0])
		{
			while (sym != ':')
			{
				fin >> sym;
			}

			if (quantumNum == 0)
			{
				fin >> quantumNum;
			}
			else
			{
				fin >> processor_ct;
			}
			sym = 'x';
		}
		else if (phrase == configVarArray[1])
		{
			while (sym != ':')
			{
				fin >> sym;
			}

			fin >> monitor_ct;
			sym = 'x';
		}
		else if (phrase == configVarArray[2])
		{
			while (sym != ':')
			{
				fin >> sym;
			}

			if (hDrive_ct == 0)
			{
				fin >> hDrive_ct;
			}
			else
			{
				fin >> HDCount;
			}
			
			
			sym = 'x';
		}
		else if (phrase == configVarArray[3])
		{
			while (sym != ':')
			{
				fin >> sym;
			}

			if (printer_ct == 0)
			{
				fin >> printer_ct;
			}
			else
			{
				fin >> printerCount;
			}
			
			sym = 'x';
		}
		else if (phrase == configVarArray[4])
		{
			while (sym != ':')
			{
				fin >> sym;
			}
			fin >> keyboard_ct;
			sym = 'x';
		}
		else if (phrase == configVarArray[5] && memory_ct == 0)
		{
			while (sym != ':')
			{
				fin >> sym;
			}
			fin >> memory_ct;
			sym = 'x';
		}
		else if (phrase == configVarArray[6])
		{
			while (sym != ':')
			{
				fin >> sym;
			}
			fin >> mouse_ct;
			sym = 'x';
		}
		else if (phrase == configVarArray[7])
		{
			while (sym != ':')
			{
				fin >> sym;
			}
			fin >> sysMemory;
			sym = 'x';
		}
		else if (phrase == configVarArray[8])
		{
			while (sym != ':')
			{
				fin >> sym;
			}

			fin >> speaker_ct;
			sym = 'x';
		}
		else if (phrase == configVarArray[9])
		{
			while (sym != ':')
			{
				fin >> sym;
			}
			fin >> blockSize;
			sym = 'x';
		}
		else if (phrase == configVarArray[10])
		{
			while (sym != ':')
			{
				fin >> sym;
			}
			fin >> cpuCode;
			sym = 'x';
		}
		
	}	

	sym = 'x';
	while(phrase != "to")
	{
		fin >> phrase;
	}
	fin >> logMF;

	while (sym != ':')
	{
		while (sym != ':')
		{
			fin >> sym;
		}
			fin >> logFileName;
	}


}

// function that prints config to console
void Configuration::printConfig()
{
	cout << fixed << setprecision(1) << endl;
	cout << "Configuration File Data: " << endl;
	cout << "Version: " << version << endl;
	cout << "Meta-Data File: " << metaDataFile << endl;
	cout << "Quantum Number: " << quantumNum << endl;
	cout << "Scheduling Code: " << cpuCode << endl;
	cout << "Processor speed = " << processor_ct << endl;
	cout << "Monitor speed = " << monitor_ct << endl;
	cout << "Hard Drive speed = " << hDrive_ct << endl;
	cout << "Printer speed = " << printer_ct << endl;
	cout << "Keyboard speed = " << keyboard_ct << endl;
	cout << "Memory speed = " << memory_ct << endl;
	cout << "Mouse speed = " << mouse_ct << endl;
	cout << "Speaker speed = " << speaker_ct << endl;
	cout << "System Memory: " << sysMemory << endl;
	cout << "Memory Block Size: " << blockSize << endl;
	cout << "Printer Count: " << printerCount << endl;
	cout << "Hard Drive Count: " << HDCount << endl;
	cout << "Log: Log to " << logMF << endl;
	cout << "Log File: " << logFileName << endl;
}

// getter and setter functions

string Configuration::getcpuCode()
{
	return cpuCode;
}

int Configuration::getquantumNum()
{
	return quantumNum;
}

double Configuration::getVersion()
{
	return version;
}

string Configuration::getMetaDataFile()
{
	return metaDataFile;
}

int Configuration::getProcessor_ct()
{
	return processor_ct;
}

int Configuration::getMonitor_ct()
{
	return monitor_ct;
}

int Configuration::getHDrive_ct()
{
	return hDrive_ct;
}
int Configuration::getPrinter_ct()
{
	return printer_ct;
}

int Configuration::getKeyboard_ct()
{
	return keyboard_ct;
}

int Configuration::getMemory_ct()
{
	return memory_ct;
}

int Configuration::getMouse_ct()
{
	return mouse_ct;
}

int Configuration::getSpeaker_ct()
{
	return speaker_ct;
}

string Configuration::getLogMF()
{
	return logMF;
}

string Configuration::getLogFileName()
{
	return logFileName;
}

int Configuration::getSysMemory()
{
	return sysMemory;
}

int Configuration::getblockSize()
{
	return blockSize;
}

int Configuration::getHDCount()
{
	return HDCount;
}

int Configuration::getprinterCount()
{
	return printerCount;
}