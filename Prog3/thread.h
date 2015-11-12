// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns 
// DUE DATE : 03/20/2015                                       
// PROGRAM ASSIGNMENT #3                                        
// FILE NAME : thread.h           
// PROGRAM PURPOSE :                                           
//    This program takes an integer and an array as command
// line arguments. The program will find the index and value
// of the first non-negative integer in the array.
//
//    This is the header file for thread.cpp.         
// ----------------------------------------------------------- 



#include <ThreadClass.h>

//initialization class
class initThread : public Thread
{
	public:
		initThread(int i, int* w);
	
	private:
		void ThreadFunc();
		int i;
		int* w;

};

//elimination class
class elimThread: public Thread
{
	public:
		elimThread(int* a, int* w, int i, int j);

	private:
		void ThreadFunc();
		int* a;
		int* w;
		int i;
		int j;
};

//conclusion class
class concThread: public Thread
{
	public:
		concThread(int i, int* w, int* p);

	private:
		void ThreadFunc();
		int i;
		int* w;
		int* p;

};
