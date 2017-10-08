/*
  Author: James Schnebly
  Title: config_class.h
  Class: CS446/646
  Date: 9/11/2017 
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Configuration
{
public:
	Configuration();
	~Configuration();

	//load the configuration file into config class
	void loadConfig(ifstream& fin);

	//print
	void printConfig();

	//getter functions
	double getVersion();
	string getMetaDataFile();
	int getProcessor_ct();
	int getMonitor_ct();
	int getHDrive_ct();
	int getPrinter_ct();
	int getKeyboard_ct();
	int getMemory_ct();
	int getMouse_ct();
	int getSpeaker_ct();
	int getSysMemory();
	string getLogMF();
	string getLogFileName();


private:
	double version;
	int processor_ct, monitor_ct, hDrive_ct, printer_ct;
	int keyboard_ct, memory_ct, mouse_ct, speaker_ct, sysMemory;
	string metaDataFile, logMF, logFileName;

};