// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns
// DUE DATE : 04/17/2015                                       
// PROGRAM ASSIGNMENT #5                                       
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//    This program simulates a game of baboons crossing a rope
//  that is suspended over a canyon. Baboons can only cross
//  when there are no baboons crossing the other way. Multiple
//  baboons can cross in the same direction.
//
//    This file is the main program that executes each baboon
//  thread in correspondence with the arguments.   
// ----------------------------------------------------------- 


#include <string.h>
#include <stdio.h>
#include "thread.h"

RopeMonitor* rope;


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
	//check for valid arguments
	if(argc != 4)
	{
		sprintf(output, "Invalid arguments. Usage: ./prog4 e w t\n");
		write(1, output, strlen(output));
		exit(0);
	}
	
	rope = new RopeMonitor();

	int e = atoi(argv[1]); //feeding pots
	int w = atoi(argv[2]); //eagles
	int t = atoi(argv[3]); //feedings
	int i;

	if(e == 0)
		e = 10;
	if(w == 0)
		w = 10;
	if(t == 0)
		t = 10;

	eastThread* east[e];
	westThread* west[w];

	sprintf(output, "MAIN: There are %d east-moving baboons, %d west-moving baboons, and %d trips.\n", e, w, t);
	write(1, output, strlen(output));

	for(i = 0; i < e; i++)
	{
		east[i] = new eastThread(i + 1, t);
		east[i]->Begin();
	}

	for(i = 0; i < w; i++)
	{
		west[i] = new westThread(i + 1, t);
		west[i]->Begin();
	}

	for(i = 0; i < e; i++)
		east[i]->Join();

	for(i = 0; i < w; i++)
		west[i]->Join();
}