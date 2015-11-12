// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns 
// DUE DATE : 02/27/15                                      
// PROGRAM ASSIGNMENT #2                                     
// FILE NAME : qsort.c           
// PROGRAM PURPOSE :                                           
//          This program is a child process that will use
// 	the quick sort algorithm to sort an array in shared
//	memory. Each split is handled by a child process.      
// ----------------------------------------------------------- 

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>

// ----------------------------------------------------------- 
// FUNCTION  main :                          
//     This function is called when the process is started.
//	It sorts an array in shared memory by quick sort.                          
// PARAMETER USAGE :                                           
//     Three parameters are received. The first is the left
// 	bound for the current process, the second is the right
//	bound, and the third is the size of the array that is
// 	to be sorted.            
// FUNCTION CALLED :                                           
//    printf, scanf, shmget, ftok, shmat, shmdt, atoi,
//	exit, execvp, wait         
// ----------------------------------------------------------- 

int main(int argc, char* argv[]){
	if(argc != 4){
		printf("Invalid arguments in qsort.\n");
		exit(1);
	}

	//generate prefix to output
	pid_t pid = getpid();
	char pre[30];
	sprintf(pre, "   ### Q-PROC(%d): ", pid);

	int i, j, shmID, pivot, temp, store, forkID;

	//read arguments
	int l = atoi(argv[1]);
	int r = atoi(argv[2]);
	int n = atoi(argv[3]);


	key_t key = ftok("./", 'q');

	struct qstruct {
		int s;
		int array[n];
	};

	//get shared memory
	if((shmID = shmget(key, sizeof(struct qstruct), 0666)) < 0){
		printf("%sFailed to create shared memory.\n", pre);
		exit(1);
	}	 

	//assign shared memory
	struct qstruct* data = shmat(shmID, NULL, 0);

	char* args[4];
	char right[16];
	char left[16];
	char size[16];
	char qprog[] = {"qsort"};

	//valid input for quick sort
	if(l < r){

		printf("%sentering with a[%d..%d]\n    ", pre, l, r);
		for(i = l; i <= r; i++)
			printf("%4d", data->array[i]);
		printf("\n       ..........\n");

		//set pivot as rightmost value
		pivot = data->array[r];
		store = l;

		printf("%spivot element is a[%d] = %d\n       ..........\n", pre, r, pivot);
		for(i = l; i <= r; i++){
			//switch a[i] and pivot if a[i] is less than a
			if(data->array[i] < pivot){
				temp = data->array[i];
				data->array[i] = data->array[store];
				data->array[store] = temp;

				store++;
			}
		}

		//move pivot into its final position
		temp = data->array[r];
		data->array[r] = data->array[store];
		data->array[store] = temp;

		//create arguments
		sprintf(size, "%d", n);

		sprintf(left, "%d", l);
		sprintf(right, "%d", store - 1);
		args[0] = qprog; args[1] = left; args[2] = right; args[3] = size; args[4] = NULL;

		//create child and execute
		forkID = fork();
		if(forkID == 0){

			if(execvp("./qsort", args) < 0){
				printf("execvp() failed\n");
				shmdt((void *) data);
				exit(1);
			}
			shmdt((void *) data);
			exit(0);
		}


		//create arguments
		sprintf(left, "%d", store + 1);
		sprintf(right, "%d", r);
		args[0] = qprog; args[1] = left; args[2] = right; args[3] = size; args[4] = NULL;

		//create child and execute
		forkID = fork();
		if(forkID == 0){

			if(execvp("./qsort", args) < 0){
				printf("execvp() failed\n");
				shmdt((void *) data);
				exit(1);
			}
			shmdt((void *) data);
			exit(0);
		}

		//wait for child processes
		wait();
		wait();

		sprintf(pre, "   ### Q-PROC(%d): ", pid);
		//print sorted section
		printf("%ssection a[%d..%d] sorted\n    ", pre, l, r);
		for(i = l; i <= r; i++)
			printf("%4d", data->array[i]);
		printf("\n       ..........\n");

		printf("%sexits\n", pre);
	}

	//detach shared memory
	shmdt((void *) data);
}