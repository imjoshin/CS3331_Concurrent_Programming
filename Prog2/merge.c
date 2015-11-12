// ----------------------------------------------------------- 
// NAME : Josh Johnson                       User ID: jocjohns 
// DUE DATE : 02/27/15                                      
// PROGRAM ASSIGNMENT #2                                    
// FILE NAME : merge.c           
// PROGRAM PURPOSE :                                           
//          This program is a child process that will merge
// 	and sort two sorted arrays. A process is forked each time
//	a position needs to be found.
// ----------------------------------------------------------- 

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>

// ----------------------------------------------------------- 
// FUNCTION  main :                          
//     This function is called when the process is started.
//	It sorts two arrays in shared memory by merge sort.                           
// PARAMETER USAGE :                                           
//     Two parameters are received: the first for the size of
//	x, and the other for the size of y.             
// FUNCTION CALLED :                                           
//    printf, scanf, shmget, ftok, shmat, shmdt, atoi,
//	exit, execvp, wait         
// ----------------------------------------------------------- 

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Invalid arguments in merge.\n");
		exit(1);
	}

	//create prefix to output
	pid_t pid = getpid();
	char pre[30];
	sprintf(pre, "      $$$ M-PROC(%d): ", pid);

	int i, j, shmID, forkID;

	//read arguments
	int xsize = atoi(argv[1]);
	int ysize = atoi(argv[2]);
	key_t key = ftok("./", 'm');

	struct mstruct {
		int xsize;
		int ysize;
		int x[xsize];
		int y[ysize];
		int ret[xsize + ysize];
	};

	//get shared memory
	if((shmID = shmget(key, sizeof(struct mstruct), 0666)) < 0){
		printf("%sFailed to create shared memory.\n", pre);
		exit(1);
	}	 

	//assign shared memory
	struct mstruct* data = shmat(shmID, NULL, 0);


	int value;
	//iterate through x
	for(i = 0; i < xsize; i++){
		forkID = fork();
		if(forkID == 0){
			
			value = data->x[i];

			//x[i] is less than all y
			if(value < data->y[0]){
				printf("%shandling x[%d] = %d\n          ..........\n", pre, i, data->x[i]);
				printf("%sx[%d] = %d is found to be smaller than y[0] = %d\n          ..........\n", pre, i, data->x[i], data->y[0]);
				printf("%sabout to write x[%d] = %d into position 0 of the output array\n          ..........\n", pre, i, data->x[i]);
				data->ret[i] = value;

			//x[i] is greater than all y
			}else if(value > data->y[ysize - 1]){
				printf("%shandling x[%d] = %d\n          ..........\n", pre, i, data->x[i]);
				printf("%sx[%d] = %d is found to be greater than y[%d] = %d\n          ..........\n", pre, i, data->x[i], xsize - 1, data->y[0]);
				printf("%sabout to write x[%d] = %d into position %d of the output array\n          ..........\n", pre, i, data->x[i], xsize + ysize - 1);
				data->ret[i + ysize] = value;

			//x[i] is between y values
			}else{
				//find where to insert
				for(j = 0; j < ysize; j++){
					if(value < data->y[j]){ data->ret[i + j] = value; break;}
				}
			}

			//detach shared memory
			shmdt((void *) data);
			exit(0);
		}
	}

	//iterate through y
	for(i = 0; i < ysize; i++){
		forkID = fork();
		if(forkID == 0){

			value = data->y[i];

			//y[i] is less than all x
			if(value < data->y[0]){
				printf("%shandling y[%d] = %d\n          ..........\n", pre, i, data->y[i]);
				printf("%sy[%d] = %d is found to be smaller than x[0] = %d\n          ..........\n", pre, i, data->y[i], data->x[0]);
				printf("%sabout to write y[%d] = %d into position 0 of the output array\n          ..........\n", pre, i, data->y[i]);
				data->ret[i] = value;

			//y[i] is greater than all x
			}else if(value > data->y[ysize - 1]){
				printf("%shandling y[%d] = %d\n          ..........\n", pre, i, data->y[i]);
				printf("%sy[%d] = %d is found to be greater than x[%d] = %d\n          ..........\n", pre, i, data->y[i], ysize - 1, data->x[0]);
				printf("%sabout to write y[%d] = %d into position %d of the output array\n          ..........\n", pre, i, data->y[i], xsize + ysize - 1);
				data->ret[i + xsize] = value;

			//y[i] is between x values
			}else{
				//find where to insert
				for(j = 0; j < xsize; j++){
					if(value < data->x[j]){ data->ret[i + j] = value; break;}
				}
			}

			//detach shared memory
			shmdt((void *) data);
			exit(0);
		}
	}

	//wait for all child processes
	for(i = 0; i < xsize + ysize; i++) wait();

	//detach shared memory
	shmdt((void *) data);
}