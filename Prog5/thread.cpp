// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns
// DUE DATE : 04/17/2015                                       
// PROGRAM ASSIGNMENT #5                                       
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    This program simulates a game of baboons crossing a rope
//  that is suspended over a canyon. Baboons can only cross
//  when there are no baboons crossing the other way. Multiple
//  baboons can cross in the same direction.
//
//    This file contains the methods of the east and west
//  moving baboons, along with the monitor's methods.  
// ----------------------------------------------------------- 

#include <string.h>
#include <stdio.h>
#include "thread.h"


// ----------------------------------------------------------- 
// FUNCTION  eastThread::eastThread :                        
//     This function is executed by any eastThread thread.
//  It initializes the values needed for this thread's job.                          
// PARAMETER USAGE :                                           
//    i - The baboon's number
//    t - How many times it should cross              
// FUNCTION CALLED :                                           
//    Threadname->seekp, sprintf       
// -----------------------------------------------------------

eastThread::eastThread(int i, int t)
{

	ThreadName.seekp(0, ios::beg);
	ThreadName << "eastwardBaboon" << i << '\0';

	//assign class variables
	this->t = t;
	this->i = i;

	int j;
	char spaces[50];

	//generate spaces
	sprintf(spaces, "");
	for(j = 0; j < i; j++)
		sprintf(spaces, "%s  ", spaces);

	sprintf(spaces, "%sEastward-moving baboon %d", spaces, i);

	//save message prefix
	sprintf(pre, "%s", spaces);
}


// ----------------------------------------------------------- 
// FUNCTION  eastThread::ThreadFunc :                        
//     This function is executed by any eastThread thread.
//  This method simulates the job of each baboon. It will
//  cross t times only when it is able to.                          
// PARAMETER USAGE :                        
//
// FUNCTION CALLED :                                           
//    sprintf, write, Delay, rope->getOnRope, rope->getOffRope
//  Exit      
// -----------------------------------------------------------

void eastThread::ThreadFunc()
{
	Thread::ThreadFunc();
	
	char output[200];
	sprintf(output, "%s started\n", pre);
	write(1, output, strlen(output));

	int i, j;
	char end[4];
	sprintf(end, "west");

	//cross t times
	for(i = 1; i <= t; i++)
	{
		for (j = 0; j < rand(); j++)
			Delay();
		
		sprintf(output, "%s arrives at the %s end\n", pre, end);
		write(1, output, strlen(output));

		rope->getOnRope(0, pre);

		for (j = 0; j < rand(); j++)
			Delay();

		rope->getOffRope(0, pre, i);

	}

	sprintf(output, "%s completes all (%d) crossings and retires. Bye-Bye!\n", pre, t);
	write(1, output, strlen(output));

	Exit();
};


// ----------------------------------------------------------- 
// FUNCTION  westThread::westThread :                        
//     This function is executed by any westThread thread.
//  It initializes the values needed for this thread's job.                          
// PARAMETER USAGE :                                           
//    i - The baboon's number
//    t - How many times it should cross              
// FUNCTION CALLED :                                           
//    Threadname->seekp, sprintf       
// -----------------------------------------------------------

westThread::westThread(int i, int t)
{

	ThreadName.seekp(0, ios::beg);
	ThreadName << "westwardBaboon" << i << '\0';

	//assign class variables
	this->t = t;
	this->i = i;

	int j;
	char spaces[50];

	//generate spaces
	sprintf(spaces, "");
	for(j = 0; j < i; j++)
		sprintf(spaces, "%s  ", spaces);

	sprintf(spaces, "%sWestward-moving baboon %d", spaces, i);

	//save message prefix
	sprintf(pre, "%s", spaces);
}


// ----------------------------------------------------------- 
// FUNCTION  westThread::ThreadFunc :                        
//     This function is executed by any westThread thread.
//  This method simulates the job of each baboon. It will
//  cross t times only when it is able to.                          
// PARAMETER USAGE :                        
//
// FUNCTION CALLED :                                           
//    sprintf, write, Delay, rope->getOnRope, rope->getOffRope
//  Exit      
// -----------------------------------------------------------

void westThread::ThreadFunc()
{
	Thread::ThreadFunc();
	
	char output[200];
	sprintf(output, "%s started\n", pre);
	write(1, output, strlen(output));

	int i, j;
	char end[4];
	sprintf(end, "east");

	//cross t times
	for(i = 1; i <= t; i++)
	{
		for (j = 0; j < rand(); j++)
			Delay();
		
		sprintf(output, "%s arrives at the %s end\n", pre, end);
		write(1, output, strlen(output));

		rope->getOnRope(1, pre);


		for (j = 0; j < rand(); j++)
			Delay();

		rope->getOffRope(1, pre, i);

	}

	sprintf(output, "%s completes all (%d) crossings and retires. Bye-Bye!\n", pre, t);
	write(1, output, strlen(output));

	Exit();
};



// ----------------------------------------------------------- 
// FUNCTION  RopeMonitor::RopeMonitor :                        
//     This function is executed by a new RopeMonitor.
//  It initializes the values needed for its job.                          
// PARAMETER USAGE :                                           
//           
// FUNCTION CALLED :                                           
//          
// -----------------------------------------------------------

RopeMonitor::RopeMonitor(): Monitor("rope", HOARE) 
{
	//assign class variables
	numBaboons = 0;
	waiting[0] = waiting[1] = 0;
	waitingLine[0] = new Condition("e2w");
	waitingLine[1] = new Condition("w2e");
};


//0 is east baboon, moving westward
//1 is west baboon, moving eastward

// ----------------------------------------------------------- 
// FUNCTION  RopeMonitor::canCross :                        
//     This function determines if it is possible to cross in
//  the specified direction.                         
// PARAMETER USAGE :                                           
//    dir - The direction to test for crossing.           
// FUNCTION CALLED :                                           
//        
// -----------------------------------------------------------

int RopeMonitor::canCross(int dir)
{
	//if none on rope, cross
	if(numBaboons == 0)
		return 1;
	//if baboons in this direction and none are waiting on the other side
	else if(this->dir == dir && waiting[1 - dir] == 0)
		return 1;
	else
		return 0;
}


// ----------------------------------------------------------- 
// FUNCTION  RopeMonitor::getOnRope :                        
//     This function allows the caller to get on the rope if
//  it is able to, otherwise makes it wait until it is allowed                         
// PARAMETER USAGE :                                           
//    dir - The direction to cross.
//    pre - The prefix to output          
// FUNCTION CALLED :                                           
//    MonitorBegin, canCross, Condition->Wait, MonitorEnd
// -----------------------------------------------------------

void RopeMonitor::getOnRope(int dir, char* pre)
{
	char output[200];
	MonitorBegin();
	if(!canCross(dir))
	{
		//begin waiting
		waiting[dir]++;
		waitingLine[dir]->Wait();
		//end waiting
		waiting[dir]--;
	}

	if(waiting[dir] > 0 && waiting[1 - dir] == 0)
		waitingLine[dir]->Signal();

	numBaboons++;
	//direction of rope is now this direction
	this->dir = dir;
	sprintf(output, "%s is on the rope\n", pre);
	write(1, output, strlen(output));
	MonitorEnd();
}


// ----------------------------------------------------------- 
// FUNCTION  RopeMonitor::getOffRope :                        
//     This function makes the caller get off the rope and
//  allows waiting baboons to cross.                        
// PARAMETER USAGE :                                           
//    dir - The direction to cross 
//    pre - The prefix to output
//    n - The number of the crossing     
// FUNCTION CALLED :                                           
//    MonitorBegin, Condition->Signal, MonitorEnd
// -----------------------------------------------------------

void RopeMonitor::getOffRope(int dir, char* pre, int n)
{
	char output[200];

	MonitorBegin();
	numBaboons--;

	sprintf(output, "%s completes crossing the canyon (%d)\n", pre, n);
	write(1, output, strlen(output));

	if(numBaboons <= 0)
	{
		//if some are waiting on the other side
		if(waiting[1 - dir] != 0){
			waitingLine[1 - dir]->Signal();
		}else{
			waitingLine[dir]->Signal();
		}
	}


	MonitorEnd();
}