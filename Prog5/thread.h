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
//    This is the header file for the program.  
// ----------------------------------------------------------- 

#include <ThreadClass.h>


class RopeMonitor: public Monitor 
{
	public:
		RopeMonitor();
		void getOnRope(int dir, char* pre);
		void getOffRope(int dir, char* pre, int n);
		int canCross(int dir);

	private:
		Condition* waitingLine[2];
		int dir;
		int numBaboons;
		int waiting[2];

};

extern RopeMonitor* rope;


class eastThread : public Thread
{
	public:
		eastThread(int i, int t);
	
	private:
		void ThreadFunc();
		int t;
		int i;
		char pre[50];
};

class westThread : public Thread
{
	public:
		westThread(int i, int t);
	
	private:
		void ThreadFunc();
		int t;
		int i;
		char pre[50];
};

