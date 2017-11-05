/*
  Author: James Schnebly
  Title: Sim04.cpp
  Class: CS446/646
  Date: 11/1/2017 
*/

#include <iostream>
#include <fstream>
#include <string> 
#include <queue>
#include <chrono>
#include <thread> // Used only for myWait function
#include <pthread.h>
#include <iomanip>
#include <semaphore.h>

#include "config_class.cpp"
#include "meta_class.cpp"
#include "MemoryFunction.c"

#define START 0
#define READY 1
#define RUNNING 2
#define WAITING 3
#define EXIT 4

using namespace std;
using namespace std::chrono;

void loadOneMetaData(ifstream& fin, queue <MetaDataNode>& metaDataQueue, char& sym);
void setupCursor(ifstream& fin);
void printMetaData(queue <MetaDataNode> metaDataQueue);
void printToLog(Configuration config, ofstream& fout);
int myWait(int ms);
void runP2(Configuration config, queue <MetaDataNode>& metaDataQueue, string x, ofstream& fout);
void* runner(void* total);

struct PCB
{
	/*
	0 = Start = process being created
	1 = Ready = waiting to be assigned to a processor
	2 = Running = procesing instrctions
	3 = Waiting = waiting for event
	4 = Exit = done running instrctions
	*/
	int processState;
	int processNum;
};

PCB P;

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

	ifstream fin;
	ofstream fout;

	Configuration config; 
	MetaDataNode mdNode;
	queue <MetaDataNode> metaDataQueue;

	string word = "";
	char parseChar = 'x';


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



// Begin Display
	cout << endl;
	cout << "Time(sec) ------ Function" << endl;
	cout << "-------------------------";
	runP2(config, metaDataQueue, config.getLogMF(),fout);
	
	pthread_exit(0);

// End Display
}

void runP2(Configuration config, queue <MetaDataNode>& metaDataQueue, string x, ofstream& fout)
{
	fout.clear();
	fout.open(config.getLogFileName());

	high_resolution_clock::time_point start = high_resolution_clock::now();

	cout << endl;

	int iterator = -1;
	P.processNum = 0;
	int PRNTRNum = 0;
	int PRNTRNumTot = config.getprinterCount();
	int HDDNum = 0;
	int HDDNumTot = config.getHDCount();

	sem_t sem;

	sem_init(&sem, 0, 1);

	while(metaDataQueue.empty() != 1)
		{
			fout << fixed << setprecision(6);
			cout << fixed << setprecision(6);

	 		char mdc = metaDataQueue.front().getHardwareChar();
			string mdd = metaDataQueue.front().getInstruction();
			int numCycles = metaDataQueue.front().getCycles();

			if (mdc != 'O' && mdc != 'P' && mdc != 'I' && mdc != 'M' && mdc != 'S' && mdc != 'A')
			{
				cout << endl;
				cout << "Error: Invalid Meta-Data code. Please check the supplied .mdf file." << endl;

				if (x == "File" || x == "Both")
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

			if (x == "File" || x == "Both")
			{
				fout << endl;
				fout << "Error: Invalid Meta-Data cycle. Please check the supplied .mdf file." << endl;
			}

			exit(0);
		}

		int total = 0;
		int ct = 0;
		string p;

		high_resolution_clock::time_point before = high_resolution_clock::now();

		if(mdd == "allocate")
		{
			ct = config.getMemory_ct();
			p = "allocating";
			duration<float> tSpan = duration_cast<duration<float>>(before - start);
			P.processState = RUNNING;
			if(x == "File" || x == "Both")
			{
				fout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << " memory" << endl;
			}
			if (x == "Monitor" || x == "Both")
			{
				cout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << " memory" << endl;
			}

			iterator++;
		}
		else if (mdd == "run")
		{
			ct = config.getProcessor_ct();
			p = "processing action";
			duration<float> tSpan = duration_cast<duration<float>>(before - start);
			P.processState = RUNNING;
			if(x == "File" || x == "Both")
			{
				fout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			if (x == "Monitor" || x == "Both")
			{
				cout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			
		} 
		else if (mdd == "harddrive")
		{
			ct = config.getHDrive_ct();
			if (mdc == 'I')
			{
				p = "hard drive input";
				total = ct * numCycles;
				void* vPtr = &total;
				pthread_t tid;
				pthread_attr_t attr;
				pthread_attr_init(&attr);
				duration<float> tSpan = duration_cast<duration<float>>(before - start);
				P.processState = RUNNING;
				if(x == "File" || x == "Both")
				{
					fout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << " on HDD " << HDDNum << endl;
				}
				if (x == "Monitor" || x == "Both")
				{
					cout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << " on HDD " << HDDNum << endl;
				}
				sem_wait(&sem);
				pthread_create(&tid, &attr, runner, vPtr);
				pthread_join(tid, NULL);
				sem_post(&sem);
				ct = -1;
				HDDNum = (HDDNum + 1) % HDDNumTot;

			}
			else
			{
				p = "hard drive output";
				total = ct * numCycles;
				void* vPtr = &total;
				pthread_t tid;
				pthread_attr_t attr;
				pthread_attr_init(&attr);
				duration<float> tSpan = duration_cast<duration<float>>(before - start);
				P.processState = RUNNING;
				if(x == "File" || x == "Both")
				{
					fout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << " on HDD " << HDDNum << endl;
				}
				if (x == "Monitor" || x == "Both")
				{
					cout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << " on HDD " << HDDNum << endl;
				}
				sem_wait(&sem);
				pthread_create(&tid, &attr, runner, vPtr);
				pthread_join(tid, NULL);
				sem_post(&sem);
				ct = -1;
				HDDNum = (HDDNum + 1) % HDDNumTot;
				
			}
			
			
		} 
		else if (mdd == "end")
		{
			ct = 0;
			if (mdc == 'S')
			{
				p = "Simulator program ending";
				duration<float> tSpan = duration_cast<duration<float>>(before - start);
				if(x == "File" || x == "Both")
				{
					fout << tSpan.count() << " - " << p << endl;
				}
				if (x == "Monitor" || x == "Both")
				{
					cout << tSpan.count() << " - " << p << endl;
				}
				
			}
			if (mdc == 'A')
			{
				p = "OS: removing Process ";
				P.processState = EXIT;
				duration<float> tSpan = duration_cast<duration<float>>(before - start);
				if(x == "File" || x == "Both")
				{
					fout << tSpan.count() << " - " << p << P.processNum <<  endl;
				}
				if (x == "Monitor" || x == "Both")
				{
					cout << tSpan.count() << " - " << p << P.processNum << endl;
				}
			}
			
		} 
		else if (mdd == "keyboard")
		{
			ct = config.getKeyboard_ct();
			p = "keyboard input";
			total = ct * numCycles;
			void* vPtr = &total;
			pthread_t tid;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			duration<float> tSpan = duration_cast<duration<float>>(before - start);
			P.processState = RUNNING;
			if(x == "File" || x == "Both")
			{
				fout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			if (x == "Monitor" || x == "Both")
			{
				cout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			sem_wait(&sem);
			pthread_create(&tid, &attr, runner, vPtr);
			pthread_join(tid, NULL);
			sem_post(&sem);
			ct = -1;
			
		} 
		else if (mdd == "printer")
		{
			ct = config.getPrinter_ct();
			p = "printer output";
			total = ct * numCycles;
			void* vPtr = &total;
			pthread_t tid;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			duration<float> tSpan = duration_cast<duration<float>>(before - start);
			P.processState = RUNNING;
			if(x == "File" || x == "Both")
			{
				fout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << " on PRNTR " << PRNTRNum << endl;
			}
			if (x == "Monitor" || x == "Both")
			{
				cout << tSpan.count() << " - Process " << P.processNum << ": Start " << p <<  " on PRNTR "  << PRNTRNum << endl;
			}
			sem_wait(&sem);
			pthread_create(&tid, &attr, runner, vPtr);
			pthread_join(tid, NULL);
			sem_post(&sem);
			ct = -1;
			PRNTRNum = (PRNTRNum + 1) % PRNTRNumTot;
			
		} 
		else if (mdd == "monitor")
		{
			ct = config.getMonitor_ct();
			p = "monitor output";
			total = ct * numCycles;
			void* vPtr = &total;
			pthread_t tid;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			duration<float> tSpan = duration_cast<duration<float>>(before - start);
			P.processState = RUNNING;
			if(x == "File" || x == "Both")
			{
				fout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			if (x == "Monitor" || x == "Both")
			{
				cout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			sem_wait(&sem);
			pthread_create(&tid, &attr, runner, vPtr);
			pthread_join(tid, NULL);
			sem_post(&sem);
			ct = -1;
			
		} 
		else if (mdd == "start")
		{
			ct = 0;
			if (mdc == 'S')
			{
				p = "Simulator program starting";
				duration<float> tSpan = duration_cast<duration<float>>(before - start);
				if(x == "File" || x == "Both")
				{
					fout << tSpan.count() << " - " << p << endl;
				}
				if (x == "Monitor" || x == "Both")
				{
					cout << tSpan.count() << " - " << p << endl;
				}
			}
			if (mdc == 'A')
			{
				P.processNum++;
				p = "OS: preparing Process ";
				P.processState = READY;
				duration<float> tSpan = duration_cast<duration<float>>(before - start);
				if(x == "File" || x == "Both")
				{
					fout << tSpan.count() << " - " << p << P.processNum << endl;
				}
				if (x == "Monitor" || x == "Both")
				{
					cout << tSpan.count() << " - " << p << P.processNum << endl;
				}
				p = "OS: starting Process ";
				P.processState = RUNNING;
				if(x == "File" || x == "Both")
				{
					fout << tSpan.count() << " - " << p << P.processNum << endl;
				}
				if (x == "Monitor" || x == "Both")
				{
					cout << tSpan.count() << " - " << p << P.processNum << endl;
				}
			}
			
		} 
		else if (mdd == "mouse")
		{
			ct = config.getMonitor_ct();
			p = "mouse input";
			total = ct * numCycles;
			void* vPtr = &total;
			pthread_t tid;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			duration<float> tSpan = duration_cast<duration<float>>(before - start);
			P.processState = RUNNING;
			if(x == "File" || x == "Both")
			{
				fout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			if (x == "Monitor" || x == "Both")
			{
				cout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			sem_wait(&sem);
			pthread_create(&tid, &attr, runner, vPtr);
			pthread_join(tid, NULL);
			sem_post(&sem);
			ct = -1;
			
		} 
		else if (mdd == "speaker")
		{
			ct = config.getSpeaker_ct();
			p = "speaker output";
			total = ct * numCycles;
			void* vPtr = &total;
			pthread_t tid;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			duration<float> tSpan = duration_cast<duration<float>>(before - start);
			P.processState = RUNNING;
			if(x == "File" || x == "Both")
			{
				fout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			if (x == "Monitor" || x == "Both")
			{
				cout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			sem_wait(&sem);
			pthread_create(&tid, &attr, runner, vPtr);
			pthread_join(tid, NULL);
			sem_post(&sem);
			ct = -1;
			
		} 
		else if (mdd == "block")
		{
			ct = config.getMemory_ct();
			p = "memory blocking";
			duration<float> tSpan = duration_cast<duration<float>>(before - start);
			P.processState = RUNNING;
			if(x == "File" || x == "Both")
			{
				fout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
			if (x == "Monitor" || x == "Both")
			{
				cout << tSpan.count() << " - Process " << P.processNum << ": Start " << p << endl;
			}
		} 
		else
		{
			cout << endl;
			cout << "Error: Invalid Meta-Data Descriptor. Please check the supplied .mdf file." << endl;
			cout << endl;

			if (x == "File" || x == "Both")
			{
				fout << endl;
				fout << "Error: Invalid Meta-Data Descriptor. Please check the supplied .mdf file." << endl;
				fout << endl;
			}
		}

			total = ct * numCycles;
		

		metaDataQueue.pop();

		if(total != 0 && (x == "Monitor" || x == "Both"))
		{
			if(ct != -1)
			{
				myWait(total);
			}
			high_resolution_clock::time_point after = high_resolution_clock::now();
			duration<float> tSpan = duration_cast<duration<float>>(after - start);

			
			if (p == "allocating")
			{
				cout << tSpan.count() << " - Process " << P.processNum << ": memory alllocated at 0x" << setfill('0') << setw(8) <<  hex << allocateMemory(config.getSysMemory(), config.getblockSize(), iterator) << endl;
				cout.copyfmt(ios(NULL));
			}
			else if(mdc != 'A' || mdc != 'S')
			{
				cout << tSpan.count() << " - Process " << P.processNum << ": End " << p << endl;
			}

		}

		
		if (total != 0 && (x == "File" || x == "Both"))
		{
			if(ct != -1)
			{
				myWait(total);
			}
			high_resolution_clock::time_point after = high_resolution_clock::now();
			duration<float> tSpan = duration_cast<duration<float>>(after - start);

			
			if (p == "allocating")
			{
				fout << tSpan.count() << " - Process " << P.processNum << ": memory alllocated at 0x" << setfill('0') << setw(8) <<  hex << allocateMemory(config.getSysMemory(), config.getblockSize(), iterator) << endl;
				fout.copyfmt(ios(NULL));
			}
			else if(mdc != 'A' || mdc != 'S')
			{
				fout << tSpan.count() << " - Process " << P.processNum << ": End " << p << endl;
			}
		}
		
	}

	fout.close();
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

int myWait(int ms)
{
	P.processState = WAITING;
	this_thread::sleep_for(milliseconds(ms));
	P.processState = RUNNING;

	return 0;
}

void* runner(void* total)
{
	int* a = (int*)total;
	myWait(*a);
	pthread_exit(0);
}