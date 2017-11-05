/*
	Author: James Schnebly
  	Title: MemoryFunction.c
 	Class: CS446/646
 	Date: 10/25/2017
*/

// PRECOMPILER DIRECTIVES //////////////////////////////////////////////////////

#ifndef MEM_FUNC_C
#define MEM_FUNC_C

// HEADER FILES ////////////////////////////////////////////////////////////////

#include "MemoryFunction.h"

// GLOBAL CONSTANTS ////////////////////////////////////////////////////////////

 // None

int allocateMemory( int totMem, int bs, int& itr )
{
	return (bs * itr) % totMem;
}

#endif // MEM_FUNC_C
