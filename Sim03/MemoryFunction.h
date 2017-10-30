/*
	Author: James Schnebly
  	Title: MemoryFunction.h
 	Class: CS446/646
 	Date: 10/25/2017
*/

// PRECOMPILER DIRECTIVES //////////////////////////////////////////////////////

#ifndef MEM_FUNC_H
#define MEM_FUNC_H

// HEADER FILES ////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// GLOBAL CONSTANTS ////////////////////////////////////////////////////////////

 // None

int allocateMemory( int totMem, int bs, int& itr);

#endif // MEM_FUNC_H
