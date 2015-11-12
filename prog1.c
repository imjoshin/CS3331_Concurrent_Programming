// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns 
// DUE DATE : 02/04/15                                      
// PROGRAM ASSIGNMENT #1                                     
// FILE NAME : prog1.c (your unix file name)            
// PROGRAM PURPOSE :                                           
//    This program creates child processes to handle multiple
//	  calculations. These calculations are heap sorting
//    integers, fibonacci number, the Buffon's Needle problem, 
//    and estimating sin.       
// ----------------------------------------------------------- 


# include <stdio.h>
# include <stdlib.h>
# include <math.h>


long fibonacci(int n){
	if(n == 0 || n == 1) return n;
	return fibonacci(n - 1) + fibonacci(n - 2);
}


void siftDown(int* array, int first, int last){
	int root = first, child, temp;

	while(root * 2 + 1 < last){
		child = 2 * root + 1;
		//if left child is less than right child and right child is in bounds
		if(array[child] < array[child + 1] && child + 1 < last){
			child+= 1;
		}

		if(array[root] < array[child]){
			//swap
			temp = array[child];
			array[child] = array[root];
			array[root] = temp;

			root = child;
		} else break; //sorted
	}
}

void heapSort(int* array, int count){
	int i, temp;

	//heapify
	for(i = (count - 2) / 2; i >= 0; i--){
		siftDown(array, i, count);
	}

	//sort
	for(i = count - 1; i > 0; i--){
		//swap
		temp = array[0];
		array[0] = array[i];
		array[i] = temp;

		siftDown(array, 0, i);
	}
}


double buffons(){

}

int main(int argc, char* argv[]){

	if(argc != 5){
		perror("Please supply 4 parameters.");
		exit(0);
	}

	//start main process and print prompts
	printf("Main Process Started\n");
	printf("Number of Random Numbers\t= %d\n", atoi(argv[1]));
	printf("Fibonacci Input\t\t\t= %d\n", atoi(argv[2]));
	printf("Buffon's Needle Interations\t= %d\n", atoi(argv[3]));
	printf("Integration Iterations\t\t= %d\n", atoi(argv[4]));


	//fork first child
	int forkID = fork();

	//FORK ERROR
	if(forkID < 0){
		perror("parent: Can't fork");
		exit(1);
	}else if(forkID > 0){
		printf("Heap Sort Process Created\n");

	//CHILD - heap sort
	}else{
		printf("   Heap Sort Process Started\n");
		printf("   Random Numbers Generated:\n");
		int* array = malloc(sizeof(int) * atoi(argv[1]));

		//randomize
		srand(time(0));
		int i;
		//print random numbers
		for(i = 0; i < atoi(argv[1]); i++){
			array[i] = rand() % 100;
			printf("   %4d", array[i]);
		}

		printf("\n   Sorted Sequence:\n");

		//sort
		heapSort(array, atoi(argv[1]));
		//print sorted numbers
		for(i = 0; i < atoi(argv[1]); i++){
			printf("   %4d", array[i]);
		}

		free(array);
		printf("\n   Heap Sort Process Exits\n");
		exit(0);
	}


	//fork second child
	forkID = fork();

	//FORK ERROR
	if(forkID < 0){
		perror("parent: Can't fork");
		exit(1);
	}else if(forkID > 0){
		printf("Fibonacci Process Created\n");

	//CHILD - fibonacci
	}else{
		printf("      Fibonacci Process Started\n");
		printf("      Input Number %d\n", atoi(argv[2]));
		printf("      Fibonacci Number f(%d) is %ld\n", atoi(argv[2]), fibonacci(atoi(argv[2])));
		printf("      Fibonacci Process Exits\n");
		exit(0);
	}


	//fork third child
	forkID = fork();

	//FORK ERROR
	if(forkID < 0){
		perror("parent: Can't fork");
		exit(1);
	}else if(forkID > 0){
		printf("Buffon's Needle Process Created\n");

	//CHILD - buffons
	}else{
		printf("         Buffon's Needle Process Started\n");
		printf("         Input Number %d\n", atoi(argv[3]));

		srand(time(0));


		int t = 0, r = atoi(argv[3]), i;
		for(i = 0; i < r; i++){
			//d = rand[0, 1)
			double d = (double) rand() / (double) (RAND_MAX);
			//a = rand[0, 2pi)
			double a = ((double) rand() / (double) (RAND_MAX) * 2 * 3.14159);

			if(d + sin(a) < 0 || d + sin(a) > 1)
				t++;
		}

		printf("         Estimated Probability is %5f\n", ((double) t)/((double) r));
		printf("         Buffon's Needle Process Exits\n");
		exit(0);
	}


	//fork fourth child
	forkID = fork();

	//FORK ERROR
	if(forkID < 0){
		perror("parent: Can't fork");
		exit(1);
	}else if(forkID > 0){
		printf("Integration Process Created\n");

	//CHILD - sin
	}else{
		printf("            Integration Process Started\n");
		printf("            Input Number %d\n", atoi(argv[4]));

		srand(time(0));

		int t = 0, i;
		for(i = 0; i < atoi(argv[4]); i++){
			//a = rand[0, pi)
			double a = ((double) rand() / (double) (RAND_MAX) * 3.14159);
			//b = rand[0, 1)
			double b = (double) rand() / (double) (RAND_MAX);

			//printf("            a = %f\n", a);
			//printf("            b = %f\n", b);
			if(b <= sin(a))
				t++;
		}

		printf("            Total Hit %d\n", t);
		//t / s * pi
		printf("            Estimated Area is %f\n", ((double) t) / ((double) atoi(argv[4])) * 3.14159);
		printf("            Integration Process Exits\n");

		exit(0);
	}

	printf("Main Process Waits\n");
	wait();
	wait();
	wait();
	wait();
	printf("Main Process Exits\n");
}