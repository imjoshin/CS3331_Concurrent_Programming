// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns 
// DUE DATE : 04/03/2015                                       
// PROGRAM ASSIGNMENT #4                                        
// FILE NAME : thread.h           
// PROGRAM PURPOSE :                                           
//    This program takes three integers as input and simulates
//  a game of baby eagles and a mother eagle. Each baby eagle
//  is its own thread and eats food in the specified number of
//  pots, which is fetched by the mother.
//
//    This is the header file for the program.      
// ----------------------------------------------------------- 

#include <ThreadClass.h>


extern Mutex* mtx;
extern Mutex* filling;
extern Semaphore* finished;
extern Semaphore* refill;
extern Semaphore* wake;
extern Semaphore* pots;

extern int numPots;
extern int numEating;
extern bool done;


class motherThread : public Thread
{
	public:
		motherThread(int T, int M);
		void goto_sleep();
		void food_ready();
		int wokenBy;
		bool retiring;
	
	private:
		void ThreadFunc();
		int t;
		int i;
		int m;
};

class babyThread : public Thread
{
	public:
		babyThread(int I, int M, motherThread* motherT);
		void ready_to_eat();
		void finish_eating();		
	
	private:
		void ThreadFunc();
		motherThread* mother;
		int i;
		int m;
		int pot;
};

void ready_to_eat(int n);

void finish_eating(int n);

void goto_sleep();

void food_ready(int i, int m);

