// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns
// DUE DATE : 4/24/15            
// PROGRAM ASSIGNMENT #6                                        
// FILE NAME : thread-main.cpp           
// PROGRAM PURPOSE :                                           
//    This program computes prime numbers using multiple
//  threads and channels between those threads. Each thread
//  created 'remembers' a prime number once it has been 
//  computed and finds the next prime number. The result of
//  the prime numbers in the range 2 to n are printed out, 
//  where n is a passed argument.
//
//    This is the main file of the program, where the master
//  thread is created.     
// ----------------------------------------------------------- 


#include <string.h>
#include <stdio.h>
#include "thread.h"

int* Primes;

// ----------------------------------------------------------- 
// FUNCTION  main :                        
//     This function is ran when the user executes the file.
//  It creates multiple threads to simulate the baboon game.                         
// PARAMETER USAGE :                                           
//    argc - the number of arguments passed
//    argv - an array of character arrays that represents all
//  command line arguments.               
// FUNCTION CALLED :                                           
//    sprintf, write, atoi, eastThread->Begin,
//  eastThread->Join, westThread->Begin, westThread->Join       
// -----------------------------------------------------------

int main(int argc, char* argv[])
{
	char output[200];
	int n;
	//check for valid arguments
	if(argc != 2)
		n = 30;
	else
		n = atoi(argv[1]);


	Primes = (int*) malloc(n * sizeof(int));

	//create master thread
	masterThread* master;
	master = new masterThread(n);
	master->Begin();

	master->Join();

	free(Primes);
}
