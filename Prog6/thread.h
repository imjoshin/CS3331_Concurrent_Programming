// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns
// DUE DATE : 4/24/15            
// PROGRAM ASSIGNMENT #6                                        
// FILE NAME : thread.h          
// PROGRAM PURPOSE :                                           
//    This program computes prime numbers using multiple
//  threads and channels between those threads. Each thread
//  created 'remembers' a prime number once it has been 
//  computed and finds the next prime number. The result of
//  the prime numbers in the range 2 to n are printed out, 
//  where n is a passed argument.
//
//    This is the thread header file.     
// ----------------------------------------------------------- 

#include <ThreadClass.h>

#define END -1

extern int* Primes;

class masterThread : public Thread
{
	public:
		masterThread(int max);
	
	private:
		void ThreadFunc();
		int max;
};
 

class pThread : public Thread
{
	public:
		pThread(int i, int n, int max);
		SynOneToOneChannel* channel;
	
	private:
		void ThreadFunc();
		int i; 			//number of thread created
		int n;			//prime number to remember
		int max;		//2 to max prime numbers
		char pre[50];
};
